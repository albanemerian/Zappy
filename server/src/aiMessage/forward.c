/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: AI Message :: Forward
*/

#include <stdio.h>
#include <stdlib.h>

#include "zappy.h"
#include "network.h"

static void move_top(player_t *player, params_t *n)
{
    player->posY -= 1;
    if (player->posY < 0)
        player->posY = n->y - 1;
}

static void move_bottom(player_t *player, params_t *n)
{
    player->posY += 1;
    if (player->posY >= n->y)
        player->posY = 0;
}

static void move_left(player_t *player, params_t *n)
{
    player->posX -= 1;
    if (player->posX < 0)
        player->posX = n->x - 1;
}

static void move_right(player_t *player, params_t *n)
{
    player->posX += 1;
    if (player->posX >= n->x)
        player->posX = 0;
}

static int print_forward_server(player_t *player)
{
    int len = int_str_len(player->id) + int_str_len(player->posX) +
        int_str_len(player->posY) + 33;
    char *debug = calloc(len + 1, sizeof(char));

    if (!debug) {
        error_message("Memory allocation failed for forward debug print.");
        return -1;
    }
    snprintf(debug, len + 1, "Player (%d) moved to position (%d, %d).",
        player->id, player->posX, player->posY);
    valid_message(debug);
    free(debug);
    return 0;
}

int forward_message(player_t *player, params_t *params)
{
    direction_t direction = player->direction;
    direction_t directions[4] = {NORTH, EAST, SOUTH, WEST};
    void (*move_functions[4])(player_t *, params_t *) = {
        &move_top, &move_right, &move_bottom, &move_left};

    if (player->posX < 0 || player->posY < 0 ||
        player->posX >= params->x || player->posY >= params->y) {
        error_message("Player position is out of bounds, resetting at (0;0).");
        player->posX = 0;
        player->posY = 0;
        return -1;
    }
    for (int i = 0; i < 4; i++) {
        if (direction == directions[i]) {
            move_functions[i](player, params);
            return print_forward_server(player);
        }
    }
    return 0;
}

/* This is the function declared int the array to handle the cmds */
int handle_forward(player_t *player, char *command, zappy_t *zappy)
{
    (void)command;
    if (forward_message(player, zappy->params) == -1)
        return -1;
    if (send_player_pos(zappy, player) == -1) {
        return -1;
    }
    return 0;
}
