/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** algo
*/

#include "algo.h"

#include <stdlib.h>
#include <stdio.h>

/* Allows the pos to be equally shuffled */
tiles_t *shuffle_fisher(int width, int heigth)
{
    int total = width * heigth;
    tiles_t *tiles = calloc(total, sizeof(tiles_t));
    tiles_t tmp = {0, 0};
    int j = 0;

    if (tiles == NULL)
        return NULL;
    for (int i = 0; i < total; i++) {
        tiles[i].x = i % width;
        tiles[i].y = i / width;
    }
    for (int i = total - 1; i > 0; i--) {
        j = rand() % (i + 1);
        tmp = tiles[i];
        tiles[i] = tiles[j];
        tiles[j] = tmp;
    }
    return tiles;
}
