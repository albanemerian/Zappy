/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_inventory
*/



#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "network.h"


static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}


// Mock variables
static char *last_written_message = NULL;
static char *last_valid_message = NULL;
static int write_message_calls = 0;
static int valid_message_calls = 0;
static int send_player_inventory_calls = 0;
static int write_message_return = 0;
static int send_player_inventory_return = 0;

// Helper functions
static player_t *create_test_player(void)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->inventory = malloc(sizeof(inventory_t));
    
    player->id = 1;
    player->posX = 5;
    player->posY = 5;
    player->level = 1;
    player->direction = NORTH;
    player->team = strdup("team1");
    player->network->fd = 42;
    player->next = NULL;
    
    // Initialize inventory with zeros
    memset(player->inventory, 0, sizeof(inventory_t));
    
    return player;
}

static zappy_t *create_test_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->game = malloc(sizeof(game_t));
    zappy->game->map = malloc(sizeof(map_t));
    zappy->game->map->width = 10;
    zappy->game->map->height = 10;
    
    return zappy;
}

static void cleanup_test_data(zappy_t *zappy, player_t *player)
{
    if (player) {
        free(player->team);
        free(player->inventory);
        free(player->network);
        free(player);
    }
    if (zappy) {
        free(zappy->game->map);
        free(zappy->game);
        free(zappy);
    }
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
    if (last_valid_message) {
        free(last_valid_message);
        last_valid_message = NULL;
    }
    write_message_calls = 0;
    valid_message_calls = 0;
    send_player_inventory_calls = 0;
    write_message_return = 0;
    send_player_inventory_return = 0;
}

// Tests for handle_inventory function
Test(inventory, handle_inventory_basic_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory";
    
    // Set up successful returns
    write_message_return = 0;
    send_player_inventory_return = 0;
    
    handle_inventory(player, command, zappy);
    
    cleanup_test_data(zappy, player);
}

Test(inventory, handle_inventory_send_player_inventory_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory";
    
    // Set send_player_inventory to fail
    write_message_return = 0;
    send_player_inventory_return = -1;
    
    int result = handle_inventory(player, command, zappy);
    
    cr_assert_eq(result, -1);

    
    cleanup_test_data(zappy, player);
}

Test(inventory, handle_inventory_empty_inventory, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory";
    
    // Inventory is already initialized with zeros
    write_message_return = 0;
    send_player_inventory_return = 0;
    
    handle_inventory(player, command, zappy);
    cleanup_test_data(zappy, player);
}

Test(inventory, handle_inventory_full_inventory, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory";
    
    // Fill inventory with various amounts
    player->inventory->nbFood = 10;
    player->inventory->nbLinemate = 5;
    player->inventory->nbDeraumere = 3;
    player->inventory->nbSibur = 7;
    player->inventory->nbMendiane = 2;
    player->inventory->nbPhiras = 1;
    player->inventory->nbThystame = 4;
    
    write_message_return = 0;
    send_player_inventory_return = 0;
    
    handle_inventory(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(inventory, handle_inventory_large_numbers, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory";
    
    // Test with large numbers to test int_str_len calculation
    player->inventory->nbFood = 999;
    player->inventory->nbLinemate = 1000;
    player->inventory->nbDeraumere = 12345;
    player->inventory->nbSibur = 0;
    player->inventory->nbMendiane = 1;
    player->inventory->nbPhiras = 99;
    player->inventory->nbThystame = 500;
    
    write_message_return = 0;
    send_player_inventory_return = 0;
    
    handle_inventory(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(inventory, handle_inventory_command_parameter_ignored, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory extra parameters ignored";
    
    write_message_return = 0;
    send_player_inventory_return = 0;
    
    handle_inventory(player, command, zappy);
}

Test(inventory, handle_inventory_zappy_parameter_ignored, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory";
    
    write_message_return = 0;
    send_player_inventory_return = 0;
    
    handle_inventory(player, command, zappy);

}

Test(inventory, inventory_message_memory_allocation, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory";
    
    // Test with varying inventory sizes to test memory allocation
    for (int i = 0; i < 5; i++) {
        player->inventory->nbFood = i * 111;
        player->inventory->nbLinemate = i * 222;
        player->inventory->nbDeraumere = i * 333;
        player->inventory->nbSibur = i * 444;
        player->inventory->nbMendiane = i * 555;
        player->inventory->nbPhiras = i * 666;
        player->inventory->nbThystame = i * 777;
        
        write_message_return = 0;
        send_player_inventory_return = 0;
        
        // Reset counters
        write_message_calls = 0;
        valid_message_calls = 0;
        send_player_inventory_calls = 0;

        handle_inventory(player, command, zappy);
        
    }
    
    cleanup_test_data(zappy, player);
}

Test(inventory, int_str_len_function_coverage, .init = redirect_all_std)
{
    // Test the int_str_len function directly
    cr_assert_eq(int_str_len(0), 1);
    cr_assert_eq(int_str_len(1), 1);
    cr_assert_eq(int_str_len(9), 1);
    cr_assert_eq(int_str_len(10), 2);
    cr_assert_eq(int_str_len(99), 2);
    cr_assert_eq(int_str_len(100), 3);
    cr_assert_eq(int_str_len(999), 3);
    cr_assert_eq(int_str_len(1000), 4);
    cr_assert_eq(int_str_len(-1), 2);
    cr_assert_eq(int_str_len(-10), 3);
    cr_assert_eq(int_str_len(-100), 4);
}

Test(inventory, handle_inventory_null_termination, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player();
    char command[] = "inventory";
    
    write_message_return = 0;
    send_player_inventory_return = 0;
    handle_inventory(player, command, zappy);
}
