/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** map_init
*/

#include "zappy.h"
#include "algo.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/* This function initialize the Game structure */
static game_t *create_game(void)
{
    game_t *game = malloc(sizeof(game_t));

    if (!game) {
        error_message("Failed to allocate memory for game structure.");
        return NULL;
    }
    game->teams = NULL;
    game->map = NULL;
    game->won = false;
    return game;
}

/* This is a function for debug purposses that prints the map tiles*/
static void print_tile(zappy_t *zappy, int x, int y)
{
    inventory_t *tile = NULL;

    if (y < 0 || y >= zappy->game->map->height ||
        x < 0 || x >= zappy->game->map->width) {
        printf("Error: Coordinates (%d,%d) out of bounds\n", x, y);
        return;
    }
    tile = &zappy->game->map->tiles[x][y];
    printf("Tile (%d,%d): F:%d L:%d D:%d S:%d M:%d P:%d T:%d\n",
        x, y, tile->nbFood, tile->nbLinemate, tile->nbDeraumere,
        tile->nbSibur, tile->nbMendiane, tile->nbPhiras, tile->nbThystame);
}

/* This function allows use to pritn the tiles */
static void print_map_tiles(zappy_t *zappy)
{
    printf("Map size: %d x %d\n", zappy->params->x, zappy->params->y);
    for (int y = 0; y < zappy->params->y; y++) {
        for (int x = 0; x < zappy->params->x; x++) {
            print_tile(zappy, x, y);
        }
    }
}

static int setup_map(zappy_t *zappy)
{
    zappy->game->map = create_map(zappy->params->x, zappy->params->y);
    if (!zappy->game->map) {
        free_zappy(zappy);
        return -1;
    }
    if (distribute_resources(zappy) == -1) {
        free_zappy(zappy);
        return -1;
    }
    return 0;
}

int init_game(zappy_t *zappy)
{
    zappy->game = create_game();
    if (!zappy->game) {
        free_zappy(zappy);
        return -1;
    }
    if (setup_map(zappy) == -1) {
        free_zappy(zappy);
        return -1;
    }
    if (init_teams(zappy) == -1) {
        free_zappy(zappy);
        return -1;
    }
    if (zappy->params->is_debug == true)
        print_map_tiles(zappy);
    if (init_egg(zappy) == -1) {
        free_zappy(zappy);
        return -1;
    }
    return 0;
}
