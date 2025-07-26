/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** handle_remove
*/

#include "zappy.h"

void rm_sibur(inventory_t *inventory)
{
    if (inventory && inventory->nbSibur > 0) {
        inventory->nbSibur--;
    }
}

void rm_mendiane(inventory_t *inventory)
{
    if (inventory && inventory->nbMendiane > 0) {
        inventory->nbMendiane--;
    }
}

void rm_phiras(inventory_t *inventory)
{
    if (inventory && inventory->nbPhiras > 0) {
        inventory->nbPhiras--;
    }
}

void rm_thystame(inventory_t *inventory)
{
    if (inventory && inventory->nbThystame > 0) {
        inventory->nbThystame--;
    }
}
