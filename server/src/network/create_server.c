/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** create_server
*/

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

#include "network.h"

int set_socket(void)
{
    int opt = 1;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Socket creation failed");
        return -1;
    }
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Set socket options failed");
        close(sockfd);
        return -1;
    }
    return sockfd;
}

int bind_socket(server_t *server)
{
    struct sockaddr_in adr;

    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = INADDR_ANY;
    adr.sin_port = htons(server->port);
    if (bind(server->sockfd, (struct sockaddr *)&adr, sizeof(adr)) < 0) {
        perror("Bind failed");
        close(server->sockfd);
        return -1;
    }
    return 0;
}

int listen_socket(server_t *server)
{
    if (listen(server->sockfd, server->backlog) < 0) {
        perror("Listen failed");
        close(server->sockfd);
        return -1;
    }
    return 0;
}
