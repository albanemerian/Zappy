/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** player_id
*/

#include "zappy.h"
#include "game.h"
#include "network.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static player_t *loop_tru_players(team_t *team, int player_id)
{
    player_t *current_player = team->players;

    while (current_player) {
        if (current_player->id == player_id) {
            return current_player;
        }
        current_player = current_player->next;
    }
    return NULL;
}

player_t *get_player_by_id(game_t *game, int player_id)
{
    team_t *current_team = game->teams;
    player_t *result = NULL;

    while (current_team) {
        result = loop_tru_players(current_team, player_id);
        if (result)
            return result;
        current_team = current_team->next;
    }
    error_message("Player not found.");
    return NULL;
}

/* This sub-function check the current id agains the other */
static int loop_thru_current_players(player_t *current_player, int highest_id)
{
    if (current_player == NULL)
        return highest_id;
    while (current_player != NULL) {
        if (current_player->id > highest_id)
            highest_id = current_player->id;
        current_player = current_player->next;
    }
    return highest_id;
}

/* This function returns the highest free id for a player */
int get_next_free_id(zappy_t *server)
{
    int highest_id = 0;
    team_t *current_team;
    player_t *current_player = NULL;

    if (!server || !server->game || !server->game->teams)
        return 1;
    current_team = server->game->teams;
    while (current_team != NULL) {
        current_player = current_team->players;
        if (current_player != NULL)
            highest_id = loop_thru_current_players(current_player, highest_id);
        current_team = current_team->next;
    }
    return highest_id + 1;
}

/* This function call the next id to get player id */
void verify_player_id(zappy_t *zappy, player_t *player)
{
    if (player->id == -1) {
        player->id = get_next_free_id(zappy);
        send_player_connect(zappy, player);
    }
}

void remove_player_by_fd(zappy_t *zappy, int fd)
{
    team_t *team = zappy->game->teams;
    player_t *player = NULL;

    while (team) {
        player = team->players;
        remove_player_from_team(team, player, fd, zappy);
        team = team->next;
    }
}
