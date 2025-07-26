/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** MCT handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static int send_mct_message(int i, graph_net_t *graphic, zappy_t *zappy)
{
    for (int j = 0; j < zappy->game->map->height; j++) {
        if (send_bct_message(graphic, i, j,
            &zappy->game->map->tiles[i][j]) == -1)
            return -1;
    }
    return 0;
}

int mct(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    if (strcmp(message, "mct") != 0) {
        error_message("Invalid mct message format.");
        return -1;
    }
    for (int i = 0; i < zappy->game->map->width; i++) {
        if (send_mct_message(i, graphic, zappy) == -1)
            return -1;
    }
    return 0;
}
