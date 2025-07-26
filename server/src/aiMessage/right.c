/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** rigth
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>

int print_right_server(player_t *player)
{
    int len = int_str_len(player->id) + 23;
    char *debug = calloc(len + 1, sizeof(char));

    if (!debug) {
        error_message("Memory allocation failed for left debug print.");
        return -1;
    }
    snprintf(debug, len + 1, "Player (%d) turned right.",
        player->id);
    valid_message(debug);
    free(debug);
    return 0;
}

int right_message(player_t *player)
{
    direction_t direction = player->direction;

    if (direction == NORTH) {
        player->direction = EAST;
        return 0;
    }
    if (direction == WEST) {
        player->direction = NORTH;
        return 0;
    }
    if (direction == SOUTH) {
        player->direction = WEST;
        return 0;
    }
    if (direction == EAST) {
        player->direction = SOUTH;
        return 0;
    } else
        return -1;
    return 0;
}

int handle_right(player_t *player, char *command, zappy_t *zappy)
{
    (void)command;
    if (right_message(player) == -1)
        return -1;
    if (send_player_pos(zappy, player) == -1)
        return -1;
    return 0;
}
