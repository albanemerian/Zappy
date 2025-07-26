/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_death_player
*/

#include "zappy.h"
#include "network.h"

#include <stdlib.h>
#include <stdio.h>

/* This function sends the gui a player as died */
int send_player_death(zappy_t *zappy, player_t *player)
{
    int xLength = int_str_len(player->id) + 7;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLength, "pdi #%d\n", player->id);
    if (zappy->params->is_debug == true)
        printf("Sending to GUI: %s", message);
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
