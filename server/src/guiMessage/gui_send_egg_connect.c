/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_egg_connect
*/

#include "network.h"
#include "zappy.h"

#include <stdlib.h>
#include <stdio.h>


/* This function notify of an player connectiong to an egg EBO */
int send_egg_connect(zappy_t *zappy, egg_t *currentEgg)
{
    int xLength = int_str_len(currentEgg->id) + 7;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLength, "ebo #%d\n", currentEgg->id);
    if (zappy->params->is_debug == true)
        printf("DEBUG: Sending egg connection message: %s", message);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, message);
        if (write_message(current->network) == -1) {
            free(message);
            return -1;
        }
        current = current->next;
    }
    free(message);
    return 0;
}
