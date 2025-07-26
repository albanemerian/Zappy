/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** BCT handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

int send_bct_message(graph_net_t *graphic, int x, int y,
    inventory_t *inventory)
{
    int size = 14 + int_str_len(x) + int_str_len(y) +
        int_str_len(inventory->nbFood) + int_str_len(inventory->nbLinemate) +
        int_str_len(inventory->nbDeraumere) + int_str_len(inventory->nbSibur)
        + int_str_len(inventory->nbMendiane) + int_str_len(inventory->nbPhiras)
        + int_str_len(inventory->nbThystame);
    char buffer[size];

    snprintf(buffer, size, "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, inventory->nbFood, inventory->nbLinemate, inventory->nbDeraumere,
        inventory->nbSibur, inventory->nbMendiane, inventory->nbPhiras,
        inventory->nbThystame);
    write_in_buffer(graphic->network->writingBuffer, buffer);
    return write_message(graphic->network);
}

int bct(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    int x = 0;
    int y = 0;

    if (strlen(message) < 7) {
        error_message("Invalid bct message format.");
        return -1;
    }
    if (sscanf(message, "bct %d %d", &x, &y) != 2) {
        error_message("Invalid bct pattern.");
        return -1;
    }
    if (x >= zappy->game->map->width || y >= zappy->game->map->height) {
        error_message("Coordinates out of bounds.");
        return -1;
    }
    return send_bct_message(graphic, x, y, &zappy->game->map->tiles[x][y]);
}
