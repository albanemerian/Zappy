/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** pis
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static void decrement_ressource(player_t *player, int ressource_id)
{
    if (ressource_id == 0)
        rm_food(player->inventory);
    if (ressource_id == 1)
        rm_linemate(player->inventory);
    if (ressource_id == 2)
        rm_deraumere(player->inventory);
    if (ressource_id == 3)
        rm_sibur(player->inventory);
    if (ressource_id == 4)
        rm_mendiane(player->inventory);
    if (ressource_id == 5)
        rm_phiras(player->inventory);
    if (ressource_id == 6)
        rm_thystame(player->inventory);
}

int pis(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    player_t *player = NULL;
    int player_id = 0;
    int ressource_id = 0;

    if (strlen(message) < 8) {
        error_message("Invalid pis message format.");
        return -1;
    }
    if (sscanf(message, "pis #%d %d", &player_id, &ressource_id) != 2) {
        error_message("Invalid pis pattern.");
        return -1;
    }
    player = get_player_by_id(zappy->game, player_id);
    if (!player || ressource_id < 0 || ressource_id >= 7) {
        error_message("Player ID or resource ID out of bounds.");
        return -1;
    }
    decrement_ressource(player, ressource_id);
    return send_pin_message(graphic, player);
}
