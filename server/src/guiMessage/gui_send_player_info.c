/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_player_info
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int send_player_connect_to_specific_gui(graph_net_t *fd, player_t *p)
{
    int xLenght = int_str_len(p->id) + int_str_len(p->posX) + int_str_len(
        p->posY) + int_str_len(p->level) + int_str_len(p->direction) +
        strlen(p->team) + 12;
    char *message = malloc(sizeof(char) * xLenght);

    if (!message)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLenght, "pnw #%d %d %d %d %d %s\n", p->id, p->posX,
        p->posY, p->direction, p->level, p->team);
    write_in_buffer(fd->network->writingBuffer, message);
    if (write_message(fd->network) == -1) {
        return -1;
    }
    free(message);
    return 0;
}

/* This function sends the Connection player PNW */
int send_player_connect(zappy_t *zappy, player_t *p)
{
    int xLenght = int_str_len(p->id) + int_str_len(p->posX) + int_str_len(
        p->posY) + int_str_len(p->level) + int_str_len(p->direction) +
        strlen(p->team) + 12;
    char *message = malloc(sizeof(char) * xLenght);
    graph_net_t *current = zappy->graph;

    if (!message)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLenght, "pnw #%d %d %d %d %d %s\n", p->id, p->posX,
        p->posY, p->direction, p->level, p->team);
    if (zappy->params->is_debug == true)
        printf("Sending to GUI: %s", message);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, message);
        if (write_message(current->network) == -1)
            return -1;
        current = current->next;
    }
    free(message);
    return 0;
}

/* This functio send the position of a player */
int send_player_pos(zappy_t *zappy, player_t *p)
{
    int xLength = int_str_len(p->id) + int_str_len(p->posX) +
        int_str_len(p->posY) + int_str_len(p->direction) + 10;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(message, xLength, "ppo #%d %d %d %d\n", p->id, p->posX, p->posY,
        p->direction);
    if (zappy->params->is_debug == true)
        printf("Sending to GUI: %s", message);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, message);
        if (write_message(current->network) == -1)
            return -1;
        current = current->next;
    }
    free(message);
    return 0;
}

/* This message allows use to send the level of a player */
int send_player_level(zappy_t *zappy, player_t *player)
{
    int xLength = int_str_len(player->id) + int_str_len(player->level) + 8;
    char *message = malloc(sizeof(char) *xLength);
    graph_net_t *current = zappy->graph;

    if (message == NULL)
        return -1;
    snprintf(message, xLength, "plv #%d %d\n", player->id, player->level);
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
