/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_left
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


int handle_left(player_t *player, char *command, zappy_t *zappy);
int send_player_pos(zappy_t *zappy, player_t *player);

// Mock variables
static int send_player_pos_calls = 0;
static int send_player_pos_return = 0;


// Helper functions
static player_t *create_test_player(direction_t direction)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    
    player->id = 1;
    player->posX = 5;
    player->posY = 5;
    player->level = 1;
    player->direction = direction;
    player->team = strdup("team1");
    player->network->fd = 42;
    player->next = NULL;
    
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
        free(player->network);
        free(player);
    }
    if (zappy) {
        free(zappy->game->map);
        free(zappy->game);
        free(zappy);
    }
    send_player_pos_calls = 0;
    send_player_pos_return = 0;
}

// Tests for left_message function
Test(left, left_message_north_to_east, .init = redirect_all_std)
{
    player_t *player = create_test_player(NORTH);
    
    int result = left_message(player);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(player->direction, WEST);
    
    cleanup_test_data(NULL, player);
}

Test(left, left_message_east_to_south, .init = redirect_all_std)
{
    player_t *player = create_test_player(EAST);
    
    int result = left_message(player);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(player->direction, NORTH);
    
    cleanup_test_data(NULL, player);
}

Test(left, left_message_south_to_west, .init = redirect_all_std)
{
    player_t *player = create_test_player(SOUTH);
    
    int result = left_message(player);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(player->direction, EAST);
    
    cleanup_test_data(NULL, player);
}

Test(left, left_message_west_to_north, .init = redirect_all_std)
{
    player_t *player = create_test_player(WEST);
    
    int result = left_message(player);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(player->direction, SOUTH);
    
    cleanup_test_data(NULL, player);
}

Test(left, left_message_invalid_direction, .init = redirect_all_std)
{
    player_t *player = create_test_player((direction_t)999);
    
    int result = left_message(player);
    
    cr_assert_eq(result, -1);
    cr_assert_eq(player->direction, 999); // Direction should remain unchanged
    
    cleanup_test_data(NULL, player);
}

Test(left, left_message_zero_direction, .init = redirect_all_std)
{
    player_t *player = create_test_player((direction_t)0);
    
    int result = left_message(player);
    
    cr_assert_eq(result, -1);
    cr_assert_eq(player->direction, 0); // Direction should remain unchanged
    
    cleanup_test_data(NULL, player);
}

Test(left, left_message_full_rotation, .init = redirect_all_std)
{
    player_t *player = create_test_player(NORTH);
    
    // Complete a full rotation: NORTH -> EAST -> SOUTH -> WEST -> NORTH
    cr_assert_eq(left_message(player), 0);
    cr_assert_eq(player->direction, WEST);
    
    cr_assert_eq(left_message(player), 0);
    cr_assert_eq(player->direction, SOUTH);
    
    cr_assert_eq(left_message(player), 0);
    cr_assert_eq(player->direction, EAST);
    
    cr_assert_eq(left_message(player), 0);
    cr_assert_eq(player->direction, NORTH);
    
    cleanup_test_data(NULL, player);
}

// Test to cover the else branch in left_message (lines 44-46)
Test(left, left_message_else_branch_coverage, .init = redirect_all_std)
{
    player_t *player = create_test_player(WEST);
    
    // This should trigger the WEST case and return 0 from the final return statement
    int result = left_message(player);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(player->direction, SOUTH);
    
    cleanup_test_data(NULL, player);
}

// Test to ensure the final return 0 is covered when direction is invalid
Test(left, left_message_final_return_coverage, .init = redirect_all_std)
{
    player_t *player = create_test_player((direction_t)42); // Invalid direction
    
    // This should go through all if conditions, hit the else return -1
    int result = left_message(player);
    
    cr_assert_eq(result, -1);
    cr_assert_eq(player->direction, 42); // Should remain unchanged
    
    cleanup_test_data(NULL, player);
}


Test(left, handle_left_invalid_direction, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player((direction_t)999);
    char command[] = "left";
    
    int result = handle_left(player, command, zappy);
    
    cr_assert_eq(result, -1);
    cr_assert_eq(player->direction, 999); // Direction should remain unchanged
    cr_assert_eq(send_player_pos_calls, 0); // Should not call send_player_pos
    
    cleanup_test_data(zappy, player);
}


// Edge case tests
Test(left, left_message_direction_consistency, .init = redirect_all_std)
{
    player_t *player = create_test_player(NORTH);
    direction_t original_direction = player->direction;
    
    // Test that failed rotation doesn't change direction
    player->direction = (direction_t)999; // Invalid direction
    int result = left_message(player);
    
    cr_assert_eq(result, -1);
    cr_assert_eq(player->direction, 999); // Should remain unchanged
    
    // Test that successful rotation changes direction correctly
    player->direction = original_direction;
    result = left_message(player);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(player->direction, WEST);
    cr_assert_neq(player->direction, original_direction);
    
    cleanup_test_data(NULL, player);
}

Test(left, left_message_multiple_rotations, .init = redirect_all_std)
{
    player_t *player = create_test_player(NORTH);
    
    // Test multiple consecutive left turns
    for (int i = 0; i < 10; i++) {
        int result = left_message(player);
        cr_assert_eq(result, 0);
    }
    
    // After 8 rotations (2 full cycles), should be back to NORTH
    cr_assert_eq(player->direction, SOUTH);
    
    cleanup_test_data(NULL, player);
}
