/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** takeObject
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int remove_from_current_tile(zappy_t *zappy, int x, int y, char *name)
{
    if (strcmp(name, "phiras") == 0) {
        if (zappy->game->map->tiles[x][y].nbPhiras > 0) {
            zappy->game->map->tiles[x][y].nbPhiras--;
            return 0;
        }
    }
    if (strcmp(name, "thystame") == 0) {
        if (zappy->game->map->tiles[x][y].nbThystame > 0) {
            zappy->game->map->tiles[x][y].nbThystame--;
            return 0;
        }
    }
    return -1;
}

static int remove_crystal_from_tile(zappy_t *zappy, int x, int y, char *name)
{
    if (strcmp(name, "sibur") == 0) {
        if (zappy->game->map->tiles[x][y].nbSibur > 0) {
            zappy->game->map->tiles[x][y].nbSibur--;
            return 0;
        }
    }
    if (strcmp(name, "mendiane") == 0) {
        if (zappy->game->map->tiles[x][y].nbMendiane > 0) {
            zappy->game->map->tiles[x][y].nbMendiane--;
            return 0;
        }
    }
    if (strcmp(name, "deraumere") == 0) {
        if (zappy->game->map->tiles[x][y].nbDeraumere > 0) {
            zappy->game->map->tiles[x][y].nbDeraumere--;
            return 0;
        }
    }
    return remove_from_current_tile(zappy, x, y, name);
}

int remove_element_from_tile(zappy_t *zappy, int x, int y, char *name)
{
    if (x < 0 || x >= zappy->game->map->width ||
        y < 0 || y >= zappy->game->map->height) {
        fprintf(stderr, "Error: Coordinates (%d,%d) out of bounds\n", x, y);
        return -1;
    }
    if (strcmp(name, "food") == 0) {
        if (zappy->game->map->tiles[x][y].nbFood > 0) {
            zappy->game->map->tiles[x][y].nbFood--;
            return 0;
        }
    }
    if (strcmp(name, "linemate") == 0) {
        if (zappy->game->map->tiles[x][y].nbLinemate > 0) {
            zappy->game->map->tiles[x][y].nbLinemate--;
            return 0;
        }
    }
    return remove_crystal_from_tile(zappy, x, y, name);
}

static int add_item_to_inventory(inventory_t *inventory, char *name)
{
    item_handler_t handlers[] = {
        {"food", add_food},
        {"linemate", add_linemate},
        {"deraumere", add_deraumere},
        {"sibur", add_sibur},
        {"mendiane", add_mendiane},
        {"phiras", add_phiras},
        {"thystame", add_thystame},
        {NULL, NULL}
    };

    if (name == NULL || inventory == NULL)
        return -1;
    for (int i = 0; handlers[i].name != NULL; i++) {
        if (strcmp(name, handlers[i].name) == 0) {
            handlers[i].add_func(inventory);
            return 0;
        }
    }
    return -1;
}

static int send_type_ressource(zappy_t *zappy, player_t *player,
    char *to)
{
    char *elems[7] = { "food", "linemate", "deraumere", "sibur", "mendiane",
        "phiras", "thystame" };
    int ressourceType[7] = {0, 1, 2, 3, 4, 5, 6};
    int type = 0;

    for (int i = 0; i < 7; i++) {
        if (strcmp(to, elems[i]) == 0) {
            type = ressourceType[i];
            break;
        }
    }
    if (send_ressource_collected(zappy, player, type) == -1)
        return -1;
    return 0;
}

/* Send the take to the ai, then sends bct, pdr, and pin */
int handle_take(player_t *player, char *command, zappy_t *zappy)
{
    char *to = strtok(command, " ");

    if (to == NULL)
        return -1;
    to = strtok(NULL, " ");
    if (to == NULL)
        return -1;
    if (remove_element_from_tile(zappy, player->posX, player->posY, to) == -1)
        return -1;
    if (add_item_to_inventory(player->inventory, to) == -1)
        return -1;
    if (send_map_tile(zappy->game->map->tiles, zappy, player->posX,
        player->posY) == -1)
        return -1;
    if (send_player_inventory(zappy, player) == -1)
        return -1;
    return send_type_ressource(zappy, player, to);
}
