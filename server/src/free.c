/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** free
*/
#include "algo.h"
#include "zappy.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void *free_params(params_t *params)
{
    if (!params)
        return NULL;
    if (params->teams) {
        for (int i = 0; i < params->nb_team; i++) {
            free(params->teams[i]);
        }
        free(params->teams);
    }
    free(params);
    return NULL;
}

void free_action_queue_pl(action_queue_t *queue)
{
    action_request_t *req = NULL;
    action_request_t *next = NULL;

    if (!queue)
        return;
    req = queue->head;
    while (req) {
        next = req->next;
        if (req->command)
            free(req->command);
        free(req);
        req = next;
    }
    free(queue);
}

static void free_game(game_t *game)
{
    if (!game)
        return;
    if (game->teams)
        free_teams(game->teams);
    if (game->map)
        free_map(game->map);
    free(game);
}

static void free_graph(graph_net_t *graph)
{
    if (!graph)
        return;
    if (graph->network == NULL)
        return;
    if (graph->network->readingBuffer == NULL
        || graph->network->writingBuffer == NULL)
        return;
    free(graph->network->readingBuffer);
    free(graph->network->writingBuffer);
    free(graph->network);
    free(graph);
}

static void free_eggs(egg_t *currentEggs)
{
    egg_t *current = currentEggs;
    egg_t *next;

    while (current) {
        next = current->next;
        if (current->teamName)
            free(current->teamName);
        free(current);
        current = next;
    }
}

void free_map(map_t *map)
{
    for (int i = 0; i < map->height; i++) {
        if (map->tiles[i]) {
            free(map->tiles[i]);
        }
    }
    free(map->tiles);
    free_eggs(map->currentEggs);
    free(map);
}

static void free_unified_poll_pl(unified_poll_t *poll_struct)
{
    if (!poll_struct)
        return;
    if (poll_struct->fds)
        free(poll_struct->fds);
    free(poll_struct);
}

void *free_zappy(zappy_t *zappy)
{
    if (!zappy)
        return NULL;
    if (zappy->params)
        free_params(zappy->params);
    if (zappy->network->sockfd != -1)
        close(zappy->network->sockfd);
    if (zappy->game)
        free_game(zappy->game);
    if (zappy->graph)
        free_graph(zappy->graph);
    if (zappy->network)
        free(zappy->network);
    free_unified_poll_pl(zappy->unified_poll);
    free(zappy);
    return NULL;
}

void *free_player(player_t *player)
{
    if (!player)
        return NULL;
    if (player->inventory)
        free(player->inventory);
    free(player);
    return NULL;
}
