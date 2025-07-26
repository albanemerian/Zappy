/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** handle_food
*/

#include "zappy.h"

void add_food(inventory_t *inventory)
{
    if (inventory) {
        inventory->nbFood++;
    }
}

void add_linemate(inventory_t *inventory)
{
    if (inventory) {
        inventory->nbLinemate++;
    }
}

void add_deraumere(inventory_t *inventory)
{
    if (inventory) {
        inventory->nbDeraumere++;
    }
}

void add_sibur(inventory_t *inventory)
{
    if (inventory) {
        inventory->nbSibur++;
    }
}

void rm_deraumere(inventory_t *inventory)
{
    if (inventory && inventory->nbDeraumere > 0) {
        inventory->nbDeraumere--;
    }
}
