/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** fork
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int get_nb_eggs(zappy_t *zappy)
{
    egg_t *current = zappy->game->map->currentEggs;
    int count = 0;

    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

void push_back_egg(zappy_t *zappy, egg_t *new)
{
    egg_t *save = zappy->game->map->currentEggs;

    if (!save) {
        zappy->game->map->currentEggs = new;
        return;
    }
    while (zappy->game->map->currentEggs->next) {
        zappy->game->map->currentEggs = zappy->game->map->currentEggs->next;
    }
    zappy->game->map->currentEggs->next = new;
    zappy->game->map->currentEggs = save;
}

static int team_allows_connection(player_t *player, zappy_t *zappy)
{
    team_t *current_team = zappy->game->teams;
    char *team_name = player->team;

    while (current_team) {
        if (strcmp(current_team->name, team_name) == 0) {
            current_team->nbEggs++;
            break;
        }
        current_team = current_team->next;
    }
    return 0;
}

/* This function defines the current runing action as incantation */
static void mark_current_action(player_t *player)
{
    if (player->current_action != NULL) {
        free(player->current_action);
        player->current_action = NULL;
    }
    player->current_action = strdup("Fork");
}

/* This function handles the initiall data of an egg being laid */
int handle_fork(player_t *player, char *command, zappy_t *zappy)
{
    int id = get_nb_eggs(zappy);
    int pos[2] = {player->posX, player->posY};
    egg_t *new = NULL;

    if (strcmp(command, "Fork") != 0) {
        error_message("Invalid command for fork handling.");
        return -1;
    }
    if (team_allows_connection(player, zappy) == -1)
        return -1;
    new = add_egg_node(id, pos, player->team, player->id);
    if (!new) {
        error_message("Failed to create a new egg.");
        return -1;
    }
    mark_current_action(player);
    push_back_egg(zappy, new);
    send_player_laying_egg(zappy, player);
    return 0;
}

/* This function handles the end of an egg laying */
int handle_fork_end(player_t *player, zappy_t *zappy)
{
    egg_t *current_egg = zappy->game->map->currentEggs;

    while (current_egg != NULL) {
        if (current_egg->idLayer == player->id && !current_egg->isHatched) {
            break;
        }
        current_egg = current_egg->next;
    }
    send_egg(zappy, current_egg);
    if (player->current_action)
        free(player->current_action);
    player->current_action = NULL;
    return 0;
}
