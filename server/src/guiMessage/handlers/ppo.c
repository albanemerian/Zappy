/*
** EPITECH PROJECT, 2025
** Zappipicaca
** File description:
** PPO handlers
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static int send_ppo_message(graph_net_t *graphic, player_t *player)
{
    int size = 10 + int_str_len(player->id) + int_str_len(player->posX) +
        int_str_len(player->posY);
    char buffer[size];

    snprintf(buffer, size + 1, "ppo #%d %d %d %d\n",
        player->id, player->posX, player->posY, player->direction);
    write_in_buffer(graphic->network->writingBuffer, buffer);
    return write_message(graphic->network);
}

int ppo(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    player_t *player = NULL;
    int player_id = 0;

    if (strlen(message) < 6) {
        error_message("Invalid ppo message format.");
        return -1;
    }
    if (sscanf(message, "ppo #%d", &player_id) != 1) {
        error_message("Invalid ppo pattern.");
        return -1;
    }
    player = get_player_by_id(zappy->game, player_id);
    if (!player)
        return -1;
    return send_ppo_message(graphic, player);
}
