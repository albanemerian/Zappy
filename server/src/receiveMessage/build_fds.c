/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** build_fds
*/

#include "zappy.h"
#include "network.h"

#include <stdlib.h>
#include <unistd.h>

static void poll_graphics_clients(zappy_t *zappy)
{
    graph_net_t *graphic = zappy->graph;

    while (graphic) {
        if (graphic->network->fd >= 0) {
            add_fd_to_poll(zappy->unified_poll, graphic->network->fd, POLLIN);
        }
        graphic = graphic->next;
    }
}

static void accept_client_fd_to_list(zappy_t *zappy, player_t *player)
{
    while (player) {
        if (player->network && player->network->fd >= 0) {
            add_fd_to_poll(zappy->unified_poll, player->network->fd, POLLIN);
        }
        player = player->next;
    }
}

static void rebuild_graphic_poll(zappy_t *zappy, player_t *player,
    int original_count, int *original_fds)
{
    team_t *team = zappy->game->teams;
    int fd = -1;

    while (team) {
        player = team->players;
        accept_client_fd_to_list(zappy, player);
        team = team->next;
    }
    poll_graphics_clients(zappy);
    for (int i = 0; i < original_count; i++) {
        fd = original_fds[i];
        if (fd != zappy->network->sockfd && is_pending_connection(zappy, fd))
            add_fd_to_poll(zappy->unified_poll, fd, POLLIN);
    }
    free(original_fds);
}

/* Revuild the list of fd to poll to have the correct nb of client */
void rebuild_poll_fds(zappy_t *zappy)
{
    player_t *player = NULL;
    int original_count = -1;
    int *original_fds = NULL;

    if (!zappy || !zappy->unified_poll)
        return;
    original_count = zappy->unified_poll->count;
    original_fds = malloc(original_count * sizeof(int));
    if (!original_fds)
        return;
    for (int i = 0; i < original_count; i++) {
        original_fds[i] = zappy->unified_poll->fds[i].fd;
    }
    zappy->unified_poll->count = 0;
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    rebuild_graphic_poll(zappy, player, original_count, original_fds);
}
