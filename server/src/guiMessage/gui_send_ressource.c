/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_ressource
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>

/* This function sends the ressoruces dropping */
int send_ressource_droped(zappy_t *zappy, player_t *player,
    int ressourceType)
{
    int xLength = int_str_len(player->id) + 9;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLength, "pdr #%d %d\n", player->id, ressourceType);
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

/* This function notify gui that the player collected a ressource  */
int send_ressource_collected(zappy_t *zappy, player_t *player,
    int ressourceType)
{
    int xLength = int_str_len(player->id) + 9;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLength, "pgt #%d %d\n", player->id, ressourceType);
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
