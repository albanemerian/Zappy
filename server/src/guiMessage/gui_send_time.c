/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_time
*/

#include "network.h"
#include "zappy.h"

#include <stdio.h>
#include <stdlib.h>

/* THhis function sends the time unit message SGT */
int send_time_message(zappy_t *zappy)
{
    int xLength = int_str_len(zappy->params->freq) + 6;
    char *message = malloc(xLength * sizeof(char));
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLength, "sgt %d\n", zappy->params->freq);
    if (zappy->params->is_debug)
        printf("Sending time message: %s", message);
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

/* This function send the updated time after the gui upt */
int send_updated_time(zappy_t *zappy, int time)
{
    int xLength = int_str_len(time) + 6;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLength, "sst %d\n", time);
    if (zappy->params->is_debug)
        printf("Sending updated time: %s", message);
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
