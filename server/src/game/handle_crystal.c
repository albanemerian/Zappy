/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** handle_crystal
*/

#include "zappy.h"

void add_mendiane(inventory_t *inventory)
{
    if (inventory) {
        inventory->nbMendiane++;
    }
}

void add_phiras(inventory_t *inventory)
{
    if (inventory) {
        inventory->nbPhiras++;
    }
}

void add_thystame(inventory_t *inventory)
{
    if (inventory) {
        inventory->nbThystame++;
    }
}

void rm_food(inventory_t *inventory)
{
    if (inventory && inventory->nbFood > 0) {
        inventory->nbFood--;
    }
}

void rm_linemate(inventory_t *inventory)
{
    if (inventory && inventory->nbLinemate > 0) {
        inventory->nbLinemate--;
    }
}
