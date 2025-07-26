/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** init_egg
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zappy.h"
#include "game.h"
#include "algo.h"

/* This function allows the creation of one egg node */
egg_t *add_egg_node(int id, int *pos, char *team_name, int id_layer)
{
    egg_t *new_egg = malloc(sizeof(egg_t));

    if (!new_egg) {
        error_message("Failed to allocate memory for new egg.");
        return NULL;
    }
    new_egg->id = id;
    new_egg->posX = pos[0];
    new_egg->posY = pos[1];
    if (team_name != NULL)
        new_egg->teamName = strdup(team_name);
    else
        new_egg->teamName = NULL;
    new_egg->idLayer = id_layer;
    new_egg->next = NULL;
    new_egg->isHatched = false;
    return new_egg;
}

/* This function allows the removall of an egg once a player connected to it */
egg_t *kil_egg_node(egg_t **head, int egg_id)
{
    egg_t *current = *head;
    egg_t *prev = NULL;

    while (current && current->id != egg_id) {
        prev = current;
        current = current->next;
    }
    if (!current)
        return *head;
    if (prev == NULL)
        *head = current->next;
    else
        prev->next = current->next;
    if (current->teamName)
        free(current->teamName);
    free(current);
    return *head;
}

/* Debug functions that allows you to print the list sent */
static void print_eggs(zappy_t *zappy)
{
    egg_t *current = zappy->game->map->currentEggs;

    if (zappy->params->is_debug == false)
        return;
    printf("Current Eggs:\n");
    while (current) {
        printf("Egg ID: %d, Position: (%d, %d), Team: %s, Layer ID: %d\n",
            current->id, current->posX, current->posY,
            current->teamName, current->idLayer);
        current = current->next;
    }
}

/* Sub function to allow to loop thrue the egg and be coding style */
static int loop_for_eggs(zappy_t *zappy, tiles_t *tiles, int *pos)
{
    egg_t *new_egg = NULL;

    for (int i = 0; i < zappy->params->nb_client *
        zappy->params->nb_team; i++) {
        pos[0] = tiles[i].x;
        pos[1] = tiles[i].y;
        new_egg = add_egg_node(i + 1, pos, NULL, -1);
        if (!new_egg) {
            error_message("Failed to create new egg node.");
            return -1;
        }
        new_egg->next = zappy->game->map->currentEggs;
        zappy->game->map->currentEggs = new_egg;
        new_egg = NULL;
    }
    return 0;
}

/* Sending function that allow the server to init the entire egg list */
int init_egg(zappy_t *zappy)
{
    tiles_t *tiles = shuffle_fisher(zappy->game->map->width,
        zappy->game->map->height);
    int *pos = malloc(sizeof(int) * 2);

    if (!pos || tiles == NULL) {
        error_message("Failed to allocate memory for egg position.");
        return -1;
    }
    if (loop_for_eggs(zappy, tiles, pos) == -1) {
        free(pos);
        free(tiles);
        return -1;
    }
    if (pos)
        free(pos);
    print_eggs(zappy);
    free(tiles);
    return 0;
}
