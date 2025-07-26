/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** MSZ handlers
*/

#include <string.h>
#include <stdio.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

int msz(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    int x = zappy->game->map->width;
    int y = zappy->game->map->height;
    int size = 7 + int_str_len(x) + int_str_len(y);
    char buffer[size];

    if (strcmp(message, "msz") != 0) {
        error_message("Invalid msz message format.");
        return -1;
    }
    snprintf(buffer, size, "msz %d %d\n", x, y);
    write_in_buffer(graphic->network->writingBuffer, buffer);
    if (write_message(graphic->network) == -1)
        return -1;
    return 0;
}
