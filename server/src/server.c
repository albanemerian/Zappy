/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** Server :: Server
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "network.h"
#include "zappy.h"

static server_t *init_network_fd(server_t *network)
{
    if (bind_socket(network) == -1) {
        close_server(network);
        free(network);
        return NULL;
    }
    if (listen_socket(network) == -1) {
        close_server(network);
        free(network);
        return NULL;
    }
    return network;
}

static server_t *init_network(int port)
{
    server_t *network = malloc(sizeof(server_t));

    if (!network) {
        error_message("Memory allocation failed for network.");
        return NULL;
    }
    network->sockfd = set_socket();
    if (network->sockfd == -1) {
        free(network);
        return NULL;
    }
    network->port = port;
    network->backlog = 10;
    network = init_network_fd(network);
    if (network == NULL)
        return NULL;
    network->pollserver.fd = network->sockfd;
    network->pollserver.events = POLLIN;
    return network;
}

static zappy_t *init_struct_poll(zappy_t *zappy)
{
    zappy->graph = NULL;
    zappy->unified_poll = init_unified_poll();
    if (!zappy->unified_poll) {
        free_zappy(zappy);
        return NULL;
    }
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    if (init_game(zappy) == -1) {
        free_zappy(zappy);
        return NULL;
    }
    return zappy;
}

zappy_t *init_server(int argc, char **argv)
{
    zappy_t *zappy = NULL;

    zappy = malloc(sizeof(zappy_t));
    if (!zappy)
        return NULL;
    zappy->params = check_args(argc, argv);
    if (!zappy->params) {
        free(zappy);
        return NULL;
    }
    zappy->network = init_network(zappy->params->port);
    if (!zappy->network) {
        free_params(zappy->params);
        free(zappy);
        return NULL;
    }
    zappy = init_struct_poll(zappy);
    return zappy;
}
