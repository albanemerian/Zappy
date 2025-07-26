/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** get_ids_player_tile
*/

#include "zappy.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void loop_thrue_player_team(int *player_list, int nb_players,
    player_t *current_player, zappy_t *zappy)
{
    for (int i = 0; i < nb_players; i++) {
        if (current_player->id == player_list[i]) {
            current_player->level += 1;
            send_player_level(zappy, current_player);
        }
    }
}

/* This function incresse the level of a player after an elevation */
void increase_level_player(int *player_list, int nb_players, zappy_t *zappy)
{
    team_t *current_team = zappy->game->teams;
    player_t *current_player = NULL;

    while (current_team != NULL) {
        current_player = current_team->players;
        while (current_player != NULL) {
            loop_thrue_player_team(player_list, nb_players, current_player,
                zappy);
            current_player = current_player->next;
        }
        current_team = current_team->next;
    }
}

static int loop_thru_player_tile(int posX, int posY, int level,
    player_t *current_player)
{
    int nb_players = 0;

    while (current_player != NULL) {
        if (current_player->posX == posX && current_player->posY == posY &&
            current_player->level == level)
            nb_players++;
        current_player = current_player->next;
    }
    return nb_players;
}

/* This functions gets the number of player on a tile */
int get_nb_player_on_tile(int posX, int posY, zappy_t *zappy, int level)
{
    int nb_players = 0;
    team_t *current_team = zappy->game->teams;
    player_t *current_player = NULL;

    while (current_team != NULL) {
        current_player = current_team->players;
        nb_players += loop_thru_player_tile(posX, posY, level, current_player);
        current_team = current_team->next;
    }
    return nb_players;
}

static void add_matching_players_to_list(int *pos,
    player_t *current_player, int *player_list, int *index)
{
    while (current_player != NULL) {
        if (current_player->posX == pos[0] && current_player->posY == pos[1]) {
            player_list[*index] = current_player->id;
            (*index)++;
        }
        current_player = current_player->next;
    }
}

/* This function returns a list of id of the current player on a tile */
int *get_player_on_tile_id(int posX, int posY, zappy_t *zappy, int nb_players)
{
    int *player_list = malloc(sizeof(int) * nb_players);
    team_t *current_team = zappy->game->teams;
    int index = 0;
    int pos[2] = {posX, posY};

    if (player_list == NULL)
        return NULL;
    while (current_team != NULL) {
        add_matching_players_to_list(pos, current_team->players,
            player_list, &index);
        current_team = current_team->next;
    }
    return player_list;
}
