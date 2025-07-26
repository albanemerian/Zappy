/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** unified_polling
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
#include <unistd.h>

static void get_message_client(player_t *player, zappy_t *zappy)
{
    char *message = get_message(player->network);

    if (message) {
        queue_action(player, message, zappy);
        free(message);
    } else {
        valid_message("Player disconnected");
        close_client(player->network);
        remove_player_by_fd(zappy, player->network->fd);
        return;
    }
}

static int handle_ai_client_message(zappy_t *zappy, int fd, player_t *player,
    short revents)
{
    while (player) {
        if (player->network && player->network->fd != fd) {
            player = player->next;
            continue;
        }
        if (revents & (POLLHUP | POLLERR | POLLNVAL)) {
            valid_message("Player disconnected");
            remove_player_by_fd(zappy, fd);
            close(fd);
            return 0;
        }
        if (revents & POLLIN) {
            get_message_client(player, zappy);
        }
        return 0;
        player = player->next;
    }
    return -1;
}

static void handle_player_fd(zappy_t *zappy, int fd, short revents)
{
    team_t *team = zappy->game->teams;
    player_t *player = NULL;

    while (team) {
        player = team->players;
        if (handle_ai_client_message(zappy, fd, player, revents) == 0)
            return;
        team = team->next;
    }
}

static void get_graphic_buffer(graph_net_t *current,
    zappy_t *zappy)
{
    char *buffer = get_message(current->network);

    if (buffer) {
        poll_graphic_commands(zappy, current, buffer);
        free(buffer);
    } else {
        valid_message("Graphic client disconnected");
        close_client(current->network);
        remove_graph_node(&zappy->graph, current->network->fd);
        return;
    }
}

static void handle_graphic_fd(zappy_t *zappy, int fd, short revents)
{
    graph_net_t *current = zappy->graph;

    while (current) {
        if (current->network->fd != fd) {
            current = current->next;
            continue;
        }
        if (revents & (POLLHUP | POLLERR | POLLNVAL)) {
            remove_graph_node(&zappy->graph, fd);
            valid_message("Graphic client disconnected");
            close(fd);
            return;
        }
        if (revents & POLLIN) {
            get_graphic_buffer(current, zappy);
        }
        return;
        current = current->next;
    }
}

static int verify_value_polls(zappy_t *zappy, int should_rebuild)
{
    int tick_duration_ms = 0;
    int poll_result = 0;

    if (!zappy || !zappy->unified_poll)
        return -1;
    if (should_rebuild)
        rebuild_poll_fds(zappy);
    if (zappy->unified_poll->count == 0)
        return -1;
    tick_duration_ms = 1000 / zappy->params->freq;
    poll_result = poll(zappy->unified_poll->fds, zappy->unified_poll->count,
        tick_duration_ms);
    if (poll_result == -1)
        return -1;
    return tick_duration_ms;
}

static int handle_pending_and_new_connection(zappy_t *zappy, int fd,
    short revents)
{
    if (fd == zappy->network->sockfd && revents & POLLIN) {
        accept_client(zappy);
        return 1;
    }
    if (is_pending_connection(zappy, fd) && revents & POLLIN) {
        accept_client_team_name(zappy, fd);
        return 1;
    }
    return 0;
}

/* This function tell use if a revuimf of the list is neccessary */
static int need_rebuild_poll_fds(zappy_t *zappy)
{
    static int last_poll_count = -1;
    int current_count = 0;

    if (zappy == NULL)
        return -1;
    current_count = zappy->unified_poll ? zappy->unified_poll->count : 0;
    if (last_poll_count != current_count) {
        last_poll_count = current_count;
        return 1;
    }
    return 0;
}

/* This funtion rebuilds and handle commands from FD'S */
void poll_all_clients(zappy_t *zappy)
{
    int should_rebuild = need_rebuild_poll_fds(zappy);
    int tick_duration_ms = verify_value_polls(zappy, should_rebuild);
    int fd = -1;
    short revents = 0;

    if (tick_duration_ms == -1 || should_rebuild == -1)
        return;
    for (int i = 0; i < zappy->unified_poll->count; i++) {
        if (zappy->unified_poll->fds[i].revents == 0)
            continue;
        fd = zappy->unified_poll->fds[i].fd;
        revents = zappy->unified_poll->fds[i].revents;
        if (handle_pending_and_new_connection(zappy, fd, revents) == 1)
            continue;
        handle_player_fd(zappy, fd, revents);
        handle_graphic_fd(zappy, fd, revents);
    }
}
