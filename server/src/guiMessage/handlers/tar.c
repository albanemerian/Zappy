/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tar
*/

#include <stdio.h>
#include <string.h>
#include "../../../include/zappy.h"
#include "../../../include/network.h"
#include "../../../include/my.h"

static void increment_ressource(zappy_t *zappy, int x, int y, int ressource_id)
{
    if (ressource_id == 0)
        add_food(&zappy->game->map->tiles[x][y]);
    if (ressource_id == 1)
        add_linemate(&zappy->game->map->tiles[x][y]);
    if (ressource_id == 2)
        add_deraumere(&zappy->game->map->tiles[x][y]);
    if (ressource_id == 3)
        add_sibur(&zappy->game->map->tiles[x][y]);
    if (ressource_id == 4)
        add_mendiane(&zappy->game->map->tiles[x][y]);
    if (ressource_id == 5)
        add_phiras(&zappy->game->map->tiles[x][y]);
    if (ressource_id == 6)
        add_thystame(&zappy->game->map->tiles[x][y]);
}

int tar(zappy_t *zappy, graph_net_t *graphic, char *message)
{
    int x = 0;
    int y = 0;
    int ressource_id = 0;

    if (strlen(message) < 9) {
        error_message("Invalid tar message format.");
        return -1;
    }
    if (sscanf(message, "tar %d %d %d", &x, &y, &ressource_id) != 3) {
        error_message("Invalid tar pattern.");
        return -1;
    }
    if (x >= zappy->game->map->width || y >= zappy->game->map->height ||
        ressource_id < 0 || ressource_id >= 7) {
        error_message("Coordinates or resource ID out of bounds.");
        return -1;
    }
    increment_ressource(zappy, x, y, ressource_id);
    return send_bct_message(graphic, x, y, &zappy->game->map->tiles[x][y]);
}
