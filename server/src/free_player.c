/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** fre__player
*/

#include "network.h"
#include "zappy.h"
#include "algo.h"

#include <stdlib.h>


void free_networkplayer(player_t *player)
{
    if (player->network) {
        if (player->network->readingBuffer)
            free(player->network->readingBuffer);
        if (player->network->writingBuffer)
            free(player->network->writingBuffer);
        free(player->network);
    }
}

void free_players(player_t *player)
{
    player_t *next;

    while (player) {
        next = player->next;
        if (player->inventory)
            free(player->inventory);
        free_networkplayer(player);
        if (player->pending_actions)
            free_action_queue_pl(player->pending_actions);
        if (player->team)
            free(player->team);
        if (player->current_action)
            free(player->current_action);
        free(player);
        player = next;
    }
}

void free_teams(team_t *teams)
{
    team_t *next;

    while (teams) {
        next = teams->next;
        if (teams->name)
            free(teams->name);
        if (teams->players)
            free_players(teams->players);
        free(teams);
        teams = next;
    }
}
