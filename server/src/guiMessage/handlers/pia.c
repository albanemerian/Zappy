/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** pia
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static void increment_ressource(player_t *player, int ressource_id)
{
    if (ressource_id == 0)
        add_food(player->inventory);
    if (ressource_id == 1)
        add_linemate(player->inventory);
    if (ressource_id == 2)
        add_deraumere(player->inventory);
    if (ressource_id == 3)
        add_sibur(player->inventory);
    if (ressource_id == 4)
        add_mendiane(player->inventory);
    if (ressource_id == 5)
        add_phiras(player->inventory);
    if (ressource_id == 6)
        add_thystame(player->inventory);
}

int pia(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    player_t *player = NULL;
    int player_id = 0;
    int ressource_id = 0;

    if (strlen(message) < 8) {
        error_message("Invalid pia message format.");
        return -1;
    }
    if (sscanf(message, "pia #%d %d", &player_id, &ressource_id) != 2) {
        error_message("Invalid pia pattern.");
        return -1;
    }
    player = get_player_by_id(zappy->game, player_id);
    if (!player || ressource_id < 0 || ressource_id >= 7) {
        error_message("Player ID or resource ID out of bounds.");
        return -1;
    }
    increment_ressource(player, ressource_id);
    return send_pin_message(graphic, player);
}
