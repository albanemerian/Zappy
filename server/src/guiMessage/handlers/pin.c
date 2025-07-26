/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** PIN handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

int send_pin_message(graph_net_t *graphic, player_t *player)
{
    int size = 16 + int_str_len(player->id) + int_str_len(player->posX) +
        int_str_len(player->posY) + int_str_len(player->inventory->nbFood) +
        int_str_len(player->inventory->nbLinemate) +
        int_str_len(player->inventory->nbDeraumere) +
        int_str_len(player->inventory->nbSibur) +
        int_str_len(player->inventory->nbMendiane) +
        int_str_len(player->inventory->nbPhiras) +
        int_str_len(player->inventory->nbThystame);
    char buffer[size];

    snprintf(buffer, size, "pin #%d %d %d %d %d %d %d %d %d %d\n", player->id,
        player->posX, player->posY, player->inventory->nbFood,
        player->inventory->nbLinemate, player->inventory->nbDeraumere,
        player->inventory->nbSibur, player->inventory->nbMendiane,
        player->inventory->nbPhiras, player->inventory->nbThystame);
    write_in_buffer(graphic->network->writingBuffer, buffer);
    return write_message(graphic->network);
}

int pin(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    player_t *player = NULL;
    int player_id = 0;

    if (strlen(message) < 6) {
        error_message("Invalid pin message format.");
        return -1;
    }
    if (sscanf(message, "pin #%d", &player_id) != 1) {
        error_message("Invalid pin pattern.");
        return -1;
    }
    player = get_player_by_id(zappy->game, player_id);
    if (!player)
        return -1;
    return send_pin_message(graphic, player);
}
