/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** SGT handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static int send_sgt_message(graph_net_t *graphic, int time)
{
    int size = 6 + int_str_len(time);
    char buffer[size];

    snprintf(buffer, size, "sgt %d\n", time);
    write_in_buffer(graphic->network->writingBuffer, buffer);
    return write_message(graphic->network);
}

int sgt(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    if (strcmp(message, "sgt") != 0) {
        error_message("Invalid sgt message format.");
        return -1;
    }
    return send_sgt_message(graphic, zappy->params->freq);
}
