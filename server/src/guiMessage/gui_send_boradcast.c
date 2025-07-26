/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_send_boradcast
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This function sends the broadcast to one player PBC */
int send_broadcast_to_player(zappy_t *zappy, player_t *player,
    const char *message)
{
    int xLength = int_str_len(player->id) + strlen(message) + 8;
    char *formatted_message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (formatted_message == NULL)
        return return_error("Failed to allocate memory for message.");
    snprintf(formatted_message, xLength, "pbc #%d %s\n", player->id, message);
    if (zappy->params->is_debug == true)
        printf("Sending to GUI: %s", formatted_message);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, formatted_message);
        if (write_message(current->network) == -1) {
            free(formatted_message);
            return return_error("Failed to write end game message.");
        }
        current = current->next;
    }
    free(formatted_message);
    return 0;
}

static int loop_thru_player(player_t *currentPlayer, zappy_t *zappy,
    const char *message)
{
    while (currentPlayer != NULL) {
        if (send_broadcast_to_player(zappy, currentPlayer, message) == -1)
            return -1;
        currentPlayer = currentPlayer->next;
    }
    return 0;
}

/* This loop send the message to all players */
int send_broadcast_to_all(zappy_t *zappy, const char *message)
{
    team_t *currentTeam = NULL;
    player_t *currentPlayer = NULL;

    for (int i = 0; i < zappy->params->nb_team; i++) {
        currentTeam = zappy->game->teams;
        currentPlayer = currentTeam->players;
        if (loop_thru_player(currentPlayer, zappy, message) == -1)
            return -1;
        currentTeam = currentTeam->next;
    }
    return 0;
}
