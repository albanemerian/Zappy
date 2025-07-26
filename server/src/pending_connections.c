/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** pending_connections
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "zappy.h"


bool is_pending_graphic(zappy_t *zappy, int fd)
{
    graph_net_t *graphic = zappy->graph;

    while (graphic) {
        if (graphic->network->fd == fd)
            return false;
        graphic = graphic->next;
    }
    return true;
}

bool is_pending_team(player_t *player, int fd)
{
    while (player) {
        if (player->network && player->network->fd == fd)
            return false;
        player = player->next;
    }
    return true;
}

bool is_pending_connection(zappy_t *zappy, int fd)
{
    team_t *team = zappy->game->teams;
    player_t *player = NULL;

    if (!zappy || !zappy->game || fd == zappy->network->sockfd)
        return false;
    while (team) {
        player = team->players;
        if (is_pending_team(player, fd) == false)
            return false;
        team = team->next;
    }
    return is_pending_graphic(zappy, fd);
}
