/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** kil
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

void remove_player_from_teamate(zappy_t *zappy, player_t *player)
{
    team_t *current_team = zappy->game->teams;
    char *team_name = player->team;

    while (current_team != NULL) {
        if (strcmp(current_team->name, team_name) == 0) {
            break;
        }
        current_team = current_team->next;
    }
    if (current_team->nbPlayerAlive > 0) {
        current_team->nbPlayerAlive--;
    }
}

static team_t *get_team_by_name(game_t *game, const char *team_name)
{
    team_t *current = game->teams;

    while (current) {
        if (strcmp(current->name, team_name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

static void kill_player(zappy_t *zappy, player_t *player)
{
    player_t *current = NULL;
    team_t *team = get_team_by_name(zappy->game, player->team);

    write_in_buffer(player->network->writingBuffer, "dead\n");
    write_message(player->network);
    remove_player_from_teamate(zappy, player);
    send_player_death(zappy, player);
    if (team->players == player) {
        team->players = player->next;
    } else {
        current = team->players;
        while (current && current->next != player) {
            current = current->next;
        }
        if (current) {
            current->next = player->next;
        }
    }
    close_client(player->network);
    free_player(player);
}

int kil(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    player_t *player = NULL;
    int player_id = 0;

    (void)graphic;
    if (strlen(message) < 6) {
        error_message("Invalid kil message format.");
        return -1;
    }
    if (sscanf(message, "kil #%d", &player_id) != 1) {
        error_message("Invalid kil pattern.");
        return -1;
    }
    player = get_player_by_id(zappy->game, player_id);
    if (!player)
        return -1;
    kill_player(zappy, player);
    return 0;
}
