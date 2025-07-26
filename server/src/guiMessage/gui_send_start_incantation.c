/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_start_incantation
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int len_player_id(int *player_list, int nb_player)
{
    int len = 0;

    for (int i = 0; i < nb_player; i++) {
        len += int_str_len(player_list[i]);
    }
    return len;
}

static int send_message_to_graphics(char *message, zappy_t *zappy)
{
    graph_net_t *current = zappy->graph;

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

/* This starts the incantations player list = id's if the players on tile */
int send_start_incantation(zappy_t *zappy, player_t *player, int *player_list,
    int nb_player)
{
    int playerIds = len_player_id(player_list, nb_player);
    int xlenght = int_str_len(player->posX) + int_str_len(player->posY) +
        int_str_len(player->level) + playerIds + 8 + (nb_player * 2);
    char *message = malloc(sizeof(char) * xlenght);
    int offset = 0;

    if (message == NULL || nb_player < 1)
        return return_error("Failed to allocate memory for string message.");
    offset = snprintf(message, xlenght, "pic %d %d %d",
        player->posX, player->posY, player->level);
    for (int i = 0; i < nb_player; i++)
        offset += snprintf(message + offset, xlenght - offset, " #%d",
            player_list[i]);
    snprintf(message + offset, xlenght - offset, "\n");
    if (zappy->params->is_debug == true)
        printf("Nb player  %d Sending to GUI: %s", nb_player, message);
    return send_message_to_graphics(message, zappy);
}
