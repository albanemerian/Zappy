/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_team_name
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "zappy.h"
#include "network.h"

/* Use the length name to create the buffer */
static int create_buffer_length(const char *team_name, zappy_t *server)
{
    int xLength = strlen(team_name) + 6;
    char *message = malloc(sizeof(char) * (xLength));
    graph_net_t *current = server->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLength, "tna %s\n", team_name);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, message);
        if (write_message(current->network) == -1)
            return -1;
        current = current->next;
    }
    free(message);
    return 0;
}

/* This function serves as the cover to send the TNA */
int send_team_name(zappy_t *server)
{
    for (int i = 0; i < server->params->nb_team; i++) {
        if (server->params->teams[i] == NULL)
            continue;
        if (create_buffer_length(server->params->teams[i], server) == -1)
            return -1;
    }
    return 0;
}
