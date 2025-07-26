/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** end_incantation
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void send_end_incantation_to_all(zappy_t *zappy, int *player_list,
    int nb_players, player_t *player)
{
    player_t *current_player = NULL;
    char msg[19];

    snprintf(msg, 19, "Current level: %d\n", player->level);
    for (int i = 0; i < nb_players; i++) {
        current_player = get_player_by_id(zappy->game, player_list[i]);
        if (current_player && current_player->network) {
            write_in_buffer(current_player->network->writingBuffer, msg);
            write_message(current_player->network);
        }
        if (current_player->current_action)
            free(current_player->current_action);
        current_player->current_action = NULL;
    }
}

/* This functions checks the params of the variable of incantation */
int handle_end_incantation(player_t *player, zappy_t *z)
{
    int *player_list = NULL;
    int nb_players = validate_and_get_players(player, z, &player_list);

    if (nb_players == -1) {
        send_end_incantation(z, player, "ko");
        return -1;
    }
    send_end_incantation(z, player, "ok");
    remove_crystal_from_tiles(player->posX, player->posY, player->level, z);
    increase_level_player(player_list, nb_players, z);
    send_end_incantation_to_all(z, player_list, nb_players, player);
    return 0;
}

static bool is_player_in_list(int player_id, int *player_list, int nb_players)
{
    for (int i = 0; i < nb_players; i++) {
        if (player_id == player_list[i])
            return true;
    }
    return false;
}

/* Function to mark players as busy during incantation */
static void mark_single_player_incanting(player_t *player, zappy_t *zappy)
{
    player->is_busy = true;
    player->remaining_cooldown = 300 / zappy->params->freq;
    if (player->current_action != NULL) {
        free(player->current_action);
        player->current_action = NULL;
    }
    player->current_action = strdup("Incantation");
}

static void process_team_players(team_t *team, int *player_list, int nb_players
    , zappy_t *zappy)
{
    player_t *current_player = team->players;

    while (current_player != NULL) {
        if (is_player_in_list(current_player->id, player_list, nb_players))
            mark_single_player_incanting(current_player, zappy);
        current_player = current_player->next;
    }
}

void mark_players_incanting(int *player_list, int nb_players, zappy_t *zappy)
{
    team_t *current_team = zappy->game->teams;

    while (current_team != NULL) {
        process_team_players(current_team, player_list, nb_players, zappy);
        current_team = current_team->next;
    }
}
