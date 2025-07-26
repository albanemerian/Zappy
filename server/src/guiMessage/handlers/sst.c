/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** SST handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static void print_sst_debug(int time)
{
    int len = int_str_len(time) + 38;
    char debug[len];

    snprintf(debug, len + 1, "SST command received. New frequency: %d.", time);
    valid_message(debug);
}

static int send_sst_message(graph_net_t *graphic, int time)
{
    int size = 6 + int_str_len(time);
    char buffer[size];

    snprintf(buffer, size, "sst %d\n", time);
    write_in_buffer(graphic->network->writingBuffer, buffer);
    return write_message(graphic->network);
}

int sst(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    int new_freq = 0;

    if (strlen(message) < 5) {
        error_message("Invalid sst message format.");
        return -1;
    }
    if (sscanf(message, "sst %d", &new_freq) != 1) {
        error_message("Invalid sst pattern.");
        return -1;
    }
    if (send_sst_message(graphic, new_freq) == -1)
        return -1;
    zappy->params->freq = new_freq;
    print_sst_debug(zappy->params->freq);
    return 0;
}
