/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** close
*/

#include <unistd.h>

#include "network.h"

void close_server(server_t *server)
{
    if (!server)
        return;
    if (server->sockfd < 0)
        return;
    close(server->sockfd);
}

void close_client(network_t *network)
{
    if (!network)
        return;
    if (network->fd < 0)
        return;
    close(network->fd);
}
