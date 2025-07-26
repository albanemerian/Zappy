/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** zappy :: Accept
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "network.h"
#include "zappy.h"
#include "network.h"

/* In the accept nedd to have a pending state of connection */
static char *check_team_name(zappy_t *zappy, int new_sockfd)
{
    char *message = get_fd_message(new_sockfd);

    if (!message) {
        error_message("Failed to read team name message from client.");
        close(new_sockfd);
        return NULL;
    }
    if (!process_new_client(message, new_sockfd, zappy)) {
        close(new_sockfd);
        return NULL;
    }
    return message;
}

static int complete_connection_rest(zappy_t *zappy, int fd,
    char *buffer, team_t *team)
{
    snprintf(buffer, 12, "%d\n", (zappy->params->nb_client + team->nbEggs)
        - team->nbPlayers);
    if (write_fd(fd, buffer) == -1) {
        free(buffer);
        return -1;
    }
    free(buffer);
    buffer = calloc(27, sizeof(char));
    if (!buffer)
        return -1;
    snprintf(buffer, 27, "%d %d\n", zappy->params->x, zappy->params->y);
    if (write_fd(fd, buffer) == -1) {
        free(buffer);
        return -1;
    }
    valid_message("New AI client connected.");
    if (buffer)
        free(buffer);
    return 0;
}

static int complete_connection(zappy_t *zappy, int fd, const char *message)
{
    char *buffer = calloc(12, sizeof(char));
    team_t *team = NULL;

    if (strcmp(message, "GRAPHIC") == 0) {
        free(buffer);
        add_fd_to_poll(zappy->unified_poll, fd, POLLIN);
        return 0;
    }
    team = add_client_to_team(message, fd, zappy);
    if (!team || !buffer) {
        if (buffer)
            free(buffer);
        return -1;
    }
    if (complete_connection_rest(zappy, fd, buffer, team) == -1) {
        free(buffer);
        return -1;
    }
    add_fd_to_poll(zappy->unified_poll, fd, POLLIN);
    return 0;
}

int accept_client_team_name(zappy_t *zappy, int new_sockfd)
{
    char *message = NULL;

    message = check_team_name(zappy, new_sockfd);
    if (!message) {
        close(new_sockfd);
        return -1;
    }
    if (complete_connection(zappy, new_sockfd, message) == -1) {
        free(message);
        close(new_sockfd);
        return -1;
    }
    free(message);
    return 0;
}

int accept_client(zappy_t *zappy)
{
    int new_sockfd = accept_connection(zappy->network);

    if (write_fd(new_sockfd, "WELCOME\n") == -1)
        return -1;
    add_fd_to_poll(zappy->unified_poll, new_sockfd, POLLIN);
    return 0;
}
