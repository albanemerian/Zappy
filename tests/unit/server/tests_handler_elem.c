/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_handler_elem
*/


#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(inventory_remove_meat, remove_meat, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbFood = 5;
    rm_food(&inventory);
    cr_assert_eq(inventory.nbFood, 4, "Expected nbFood to be 4, got %d", inventory.nbFood);
}

Test(inventory_remove_linemate, remove_linemate, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbLinemate = 3;
    rm_linemate(&inventory);
    cr_assert_eq(inventory.nbLinemate, 2, "Expected nbLinemate to be 2, got %d", inventory.nbLinemate);
}

Test(inventory_remove_deraumere, remove_deraumere, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbDeraumere = 2;
    rm_deraumere(&inventory);
    cr_assert_eq(inventory.nbDeraumere, 1, "Expected nbDeraumere to be 1, got %d", inventory.nbDeraumere);
}

Test(inventory_remove_sibur, remove_sibur, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbSibur = 4;
    rm_sibur(&inventory);
    cr_assert_eq(inventory.nbSibur, 3, "Expected nbSibur to be 3, got %d", inventory.nbSibur);
}

Test(inventory_remove_mendiane, remove_mendiane, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbMendiane = 6;
    rm_mendiane(&inventory);
    cr_assert_eq(inventory.nbMendiane, 5, "Expected nbMendiane to be 5, got %d", inventory.nbMendiane);
}

Test(inventory_remove_phiras, remove_phiras, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbPhiras = 1;
    rm_phiras(&inventory);
    cr_assert_eq(inventory.nbPhiras, 0, "Expected nbPhiras to be 0, got %d", inventory.nbPhiras);
}

Test(inventory_remove_thystame, remove_thystame, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbThystame = 10;
    rm_thystame(&inventory);
    cr_assert_eq(inventory.nbThystame, 9, "Expected nbThystame to be 9, got %d", inventory.nbThystame);
}

Test(inventory_remove_food_zero, remove_food_zero, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbFood = 0;
    rm_food(&inventory);
    cr_assert_eq(inventory.nbFood, 0, "Expected nbFood to remain 0, got %d", inventory.nbFood);
}

Test(inventory_remove_linemate_zero, remove_linemate_zero, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbLinemate = 0;
    rm_linemate(&inventory);
    cr_assert_eq(inventory.nbLinemate, 0, "Expected nbLinemate to remain 0, got %d", inventory.nbLinemate);
}

Test(inventory_remove_deraumere_zero, remove_deraumere_zero, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbDeraumere = 0;
    rm_deraumere(&inventory);
    cr_assert_eq(inventory.nbDeraumere, 0, "Expected nbDeraumere to remain 0, got %d", inventory.nbDeraumere);
}

Test(inventory_remove_sibur_zero, remove_sibur_zero, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbSibur = 0;
    rm_sibur(&inventory);
    cr_assert_eq(inventory.nbSibur, 0, "Expected nbSibur to remain 0, got %d", inventory.nbSibur);
}

Test(inventory_remove_mendiane_zero, remove_mendiane_zero, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbMendiane = 0;
    rm_mendiane(&inventory);
    cr_assert_eq(inventory.nbMendiane, 0, "Expected nbMendiane to remain 0, got %d", inventory.nbMendiane);
}

Test(inventory_remove_phiras_zero, remove_phiras_zero, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbPhiras = 0;
    rm_phiras(&inventory);
    cr_assert_eq(inventory.nbPhiras, 0, "Expected nbPhiras to remain 0, got %d", inventory.nbPhiras);
}

Test(inventory_remove_thystame_zero, remove_thystame_zero, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    inventory.nbThystame = 0;
    rm_thystame(&inventory);
    cr_assert_eq(inventory.nbThystame, 0, "Expected nbThystame to remain 0, got %d", inventory.nbThystame);
}

Test(add_deamroe, add_deraumere, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    add_deraumere(&inventory);
    cr_assert_eq(inventory.nbDeraumere, 1, "Expected nbDeraumere to be 1, got %d", inventory.nbDeraumere);
}

Test(add_sibur, add_sibur, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    add_sibur(&inventory);
    cr_assert_eq(inventory.nbSibur, 1, "Expected nbSibur to be 1, got %d", inventory.nbSibur);
}

Test(add_mendiane, add_mendiane, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    add_mendiane(&inventory);
    cr_assert_eq(inventory.nbMendiane, 1, "Expected nbMendiane to be 1, got %d", inventory.nbMendiane);
}

Test(add_phiras, add_phiras, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    add_phiras(&inventory);
    cr_assert_eq(inventory.nbPhiras, 1, "Expected nbPhiras to be 1, got %d", inventory.nbPhiras);
}

Test(add_thystame, add_thystame, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    add_thystame(&inventory);
    cr_assert_eq(inventory.nbThystame, 1, "Expected nbThystame to be 1, got %d", inventory.nbThystame);
}

Test(add_food, add_food, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    add_food(&inventory);
    cr_assert_eq(inventory.nbFood, 1, "Expected nbFood to be 1, got %d", inventory.nbFood);
}

Test(add_linemate, add_linemate, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    add_linemate(&inventory);
    cr_assert_eq(inventory.nbLinemate, 1, "Expected nbLinemate to be 1, got %d", inventory.nbLinemate);
}

Test(add_food_multiple, add_food_multiple, .init = redirect_all_std)
{
    inventory_t inventory = {0};

    for (int i = 0; i < 5; i++) {
        add_food(&inventory);
    }
    cr_assert_eq(inventory.nbFood, 5, "Expected nbFood to be 5, got %d", inventory.nbFood);
}
