/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_end_incantation
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Here in this function we send the pie message result is ok or ko*/
int send_end_incantation(zappy_t *zappy, player_t *player, char *result)
{
    int xLength = int_str_len(player->posX) + int_str_len(player->posY) + 11;
    char *m = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;
    int r = (strcmp(result, "ok") == 0) ? 1 : 0;

    if (m == NULL)
        return return_error("Failed to allocate memory for string m.");
    snprintf(m, xLength, "pie %d %d %d\n", player->posX, player->posY, r);
    if (zappy->params->is_debug == true)
        printf("Sending to GUI: %s", m);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, m);
        if (write_message(current->network) == -1) {
            free(m);
            return -1;
        }
        current = current->next;
    }
    free(m);
    return 0;
}
