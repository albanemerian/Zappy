/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Init map
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"
#include "game.h"
#include "algo.h"

/* This function distribute a rzndom position from the tiles */
static int *distrib_tiles(int *tile_index, tiles_t *shuffled_tiles,
    int mapValue)
{
    int *pos = malloc(sizeof(int) * 2);

    if (pos == NULL) {
        error_message("Failed to allocate memory for tile position.");
        exit(84);
    }
    if (*tile_index >= mapValue) {
        *tile_index = 0;
    }
    pos[0] = shuffled_tiles[*tile_index].x;
    pos[1] = shuffled_tiles[*tile_index].y;
    (*tile_index)++;
    return pos;
}

/* This function initialize the tiles of the map */
static map_t *set_tile(int x, int y, map_t *map, int type)
{
    inventory_t *tile = &map->tiles[x][y];
    int *counters[] = { &tile->nbFood, &tile->nbLinemate, &tile->nbDeraumere,
        &tile->nbSibur, &tile->nbMendiane, &tile->nbPhiras, &tile->nbThystame};

    if (type >= 0 && type < 7)
        (*counters[type])++;
    return map;
}

static int *get_needed_amount_of_ressources(float *density, int mapValue,
    zappy_t *z)
{
    int required_count[7];
    int current_count[7];
    int *needed_count = malloc(sizeof(int) * 7);

    if (needed_count == NULL)
        return NULL;
    for (int i = 0; i < 7; i++) {
        required_count[i] = (int)(mapValue * density[i]);
        if (required_count[i] <= 0)
            required_count[i] = 1;
    }
    count_current_resources(z, current_count);
    for (int i = 0; i < 7; i++) {
        needed_count[i] = required_count[i] - current_count[i];
        if (needed_count[i] < 0)
            needed_count[i] = 0;
    }
    return needed_count;
}

/* This function distrbute the ressources on the tiles */
int distribute_resources(zappy_t *z)
{
    int *pos = NULL;
    int mapValue = z->params->x * z->params->y;
    float density[7] = {0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05};
    tiles_t *shuffled_tiles = shuffle_fisher(z->params->x, z->params->y);
    int *needed_count = get_needed_amount_of_ressources(density, mapValue, z);
    int tile_index = 0;

    if (shuffled_tiles == NULL || needed_count == NULL)
        return -1;
    for (int type = 0; type < 7; type++) {
        for (int count = 0; count < needed_count[type]; count++) {
            pos = distrib_tiles(&tile_index, shuffled_tiles, mapValue);
            z->game->map = set_tile(pos[0], pos[1], z->game->map, type);
            send_map_tile(z->game->map->tiles, z, pos[0], pos[1]);
            free(pos);
        }
    }
    free(needed_count);
    free(shuffled_tiles);
    return 0;
}

static map_t *malloc_tiles(int width, int height,
    map_t *map)
{
    map->currentEggs = NULL;
    for (int i = 0; i < width; i++) {
        map->tiles[i] = calloc(height, sizeof(inventory_t));
        if (!map->tiles[i]) {
            error_message("Failed to allocate memory for map row.");
            free_map(map);
            return NULL;
        }
    }
    return map;
}

map_t *create_map(int width, int height)
{
    map_t *map = malloc(sizeof(map_t));

    if (!map) {
        error_message("Failed to allocate memory for map structure.");
        return NULL;
    }
    map->width = width;
    map->height = height;
    map->tiles = malloc(sizeof(inventory_t *) * width);
    if (!map->tiles) {
        error_message("Failed to allocate memory for map tiles.");
        free_map(map);
        return NULL;
    }
    map = malloc_tiles(width, height, map);
    if (!map)
        return NULL;
    return map;
}
