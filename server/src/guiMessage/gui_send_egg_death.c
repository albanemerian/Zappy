/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_egg_death
*/


#include "zappy.h"
#include "network.h"

#include <stdlib.h>
#include <stdio.h>

int send_egg_death(zappy_t *zappy, egg_t *egg)
{
    int xlenght = int_str_len(egg->id) + 7;
    char *message = malloc(sizeof(char) * xlenght);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xlenght, "edi #%d\n", egg->id);
    if (zappy->params->is_debug)
        printf("Sending egg death: %s", message);
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
