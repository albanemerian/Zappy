/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_look
*/


#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "network.h"

// Function declarations
int wrap(int value, int max);
char *look_up(player_t *player, zappy_t *zappy);
int handle_look(player_t *player, char *command, zappy_t *zappy);

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


static zappy_t *create_test_zappy(int width, int height)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->game = malloc(sizeof(game_t));
    zappy->game->map = malloc(sizeof(map_t));
    zappy->game->map->width = width;
    zappy->game->map->height = height;
    
    // Allocate tiles array
    zappy->game->map->tiles = malloc(height * sizeof(inventory_t *));
    for (int i = 0; i < height; i++) {
        zappy->game->map->tiles[i] = calloc(width, sizeof(inventory_t));
    }
    
    // Initialize teams
    zappy->game->teams = malloc(sizeof(team_t));
    zappy->game->teams->name = strdup("team1");
    zappy->game->teams->players = NULL;
    zappy->game->teams->next = NULL;
    
    return zappy;
}

static player_t *create_test_player(int x, int y, int level, direction_t dir)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->inventory = malloc(sizeof(inventory_t));
    
    player->id = 1;
    player->posX = x;
    player->posY = y;
    player->level = level;
    player->direction = dir;
    player->team = strdup("team1");
    player->network->fd = 42;
    player->next = NULL;
    
    // Initialize inventory
    memset(player->inventory, 0, sizeof(inventory_t));
    
    return player;
}

static void cleanup_test_data(zappy_t *zappy, player_t *player)
{
    if (zappy) {
        for (int i = 0; i < zappy->game->map->height; i++) {
            free(zappy->game->map->tiles[i]);
        }
        free(zappy->game->map->tiles);
        free(zappy->game->map);
        free(zappy->game->teams->name);
        free(zappy->game->teams);
        free(zappy->game);
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player->inventory);
        free(player->network);
        free(player);
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
}

// Tests for wrap function
Test(look, wrap_positive_values, .init = redirect_all_std)
{
    cr_assert_eq(wrap(5, 10), 5);
    cr_assert_eq(wrap(0, 10), 0);
    cr_assert_eq(wrap(9, 10), 9);
    cr_assert_eq(wrap(15, 10), 5);
    cr_assert_eq(wrap(23, 10), 3);
}

Test(look, wrap_negative_values, .init = redirect_all_std)
{
    cr_assert_eq(wrap(-1, 10), 9);
    cr_assert_eq(wrap(-5, 10), 5);
    cr_assert_eq(wrap(-10, 10), 0);
    cr_assert_eq(wrap(-15, 10), 5);
    cr_assert_eq(wrap(-23, 10), 7);
}

Test(look, wrap_edge_cases, .init = redirect_all_std)
{
    cr_assert_eq(wrap(0, 1), 0);
    cr_assert_eq(wrap(1, 1), 0);
    cr_assert_eq(wrap(-1, 1), 0);
    cr_assert_eq(wrap(0, 5), 0);
    cr_assert_eq(wrap(5, 5), 0);
}

// Tests for look_up function
Test(look, look_up_level_0_basic, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    cr_assert(strstr(result, "[ player") != NULL);
    cr_assert(strstr(result, "]") != NULL);
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_level_1_multiple_tiles, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 1, NORTH);
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    cr_assert(strstr(result, "[ player") != NULL);
    // Level 1 should have multiple tiles
    int comma_count = 0;
    for (char *p = result; *p; p++) {
        if (*p == ',') comma_count++;
    }
    cr_assert_geq(comma_count, 3); // Should have at least 4 tiles
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_with_resources_on_tile, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    
    // Add resources to player's tile
    zappy->game->map->tiles[5][5].nbFood = 2;
    zappy->game->map->tiles[5][5].nbLinemate = 1;
    zappy->game->map->tiles[5][5].nbDeraumere = 1;
    zappy->game->map->tiles[5][5].nbSibur = 1;
    zappy->game->map->tiles[5][5].nbMendiane = 1;
    zappy->game->map->tiles[5][5].nbPhiras = 1;
    zappy->game->map->tiles[5][5].nbThystame = 1;
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    cr_assert(strstr(result, "food") != NULL);
    cr_assert(strstr(result, "linemate") != NULL);
    cr_assert(strstr(result, "deraumere") != NULL);
    cr_assert(strstr(result, "sibur") != NULL);
    cr_assert(strstr(result, "mendiane") != NULL);
    cr_assert(strstr(result, "phiras") != NULL);
    cr_assert(strstr(result, "thystame") != NULL);
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_multiple_same_resources, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    
    // Add multiple food items
    zappy->game->map->tiles[5][5].nbFood = 3;
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    // Count occurrences of "food"
    int food_count = 0;
    char *ptr = result;
    while ((ptr = strstr(ptr, "food")) != NULL) {
        food_count++;
        ptr += 4;
    }
    cr_assert_eq(food_count, 3);
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_with_other_players_same_team, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    player_t *other_player = create_test_player(5, 5, 0, NORTH);
    
    // Add other player to same team
    zappy->game->teams->players = other_player;
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    // Should contain multiple "player" mentions
    int player_count = 0;
    char *ptr = result;
    while ((ptr = strstr(ptr, "player")) != NULL) {
        player_count++;
        ptr += 6;
    }
    cr_assert_geq(player_count, 2); // At least current player + other player
    
    free(result);
    free(other_player->team);
    free(other_player->inventory);
    free(other_player->network);
    free(other_player);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_with_multiple_teams, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    player_t *team2_player = create_test_player(5, 5, 0, NORTH);
    
    // Create second team
    team_t *team2 = malloc(sizeof(team_t));
    team2->name = strdup("team2");
    team2->players = team2_player;
    team2->next = NULL;
    zappy->game->teams->next = team2;
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    
    free(result);
    free(team2_player->team);
    free(team2_player->inventory);
    free(team2_player->network);
    free(team2_player);
    free(team2->name);
    free(team2);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_map_wrapping_corner, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(5, 5);
    player_t *player = create_test_player(0, 0, 1, NORTH);
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    cr_assert(strstr(result, "[ player") != NULL);
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_map_wrapping_edge, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(5, 5);
    player_t *player = create_test_player(4, 4, 1, NORTH);
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    cr_assert(strstr(result, "[ player") != NULL);
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_high_level_player, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(20, 20);
    player_t *player = create_test_player(10, 10, 3, NORTH);
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    cr_assert(strstr(result, "[ player") != NULL);
    // High level should see many tiles
    int comma_count = 0;
    for (char *p = result; *p; p++) {
        if (*p == ',') comma_count++;
    }
    cr_assert_geq(comma_count, 15); // Level 3 should see many tiles
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_small_map, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(3, 3);
    player_t *player = create_test_player(1, 1, 2, NORTH);
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    cr_assert(strstr(result, "[ player") != NULL);
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, look_up_player_not_on_same_tile, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    player_t *other_player = create_test_player(3, 3, 0, NORTH);
    
    zappy->game->teams->players = other_player;
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    // For level 0, there should only be one tile (the player's tile)
    // So no comma should be present since it's only one tile
    char *first_comma = strchr(result, ',');
    if (first_comma != NULL) {
        char first_tile[256];
        strncpy(first_tile, result, first_comma - result);
        first_tile[first_comma - result] = '\0';
        
        int player_count = 0;
        char *ptr = first_tile;
        while ((ptr = strstr(ptr, "player")) != NULL) {
            player_count++;
            ptr += 6;
        }
        cr_assert_eq(player_count, 1); // Only current player on his tile
    } else {
        // Level 0 player should only see their own tile, so no comma
        int player_count = 0;
        char *ptr = result;
        while ((ptr = strstr(ptr, "player")) != NULL) {
            player_count++;
            ptr += 6;
        }
        cr_assert_eq(player_count, 1); // Only current player
    }
    
    free(result);
    free(other_player->team);
    free(other_player->inventory);
    free(other_player->network);
    free(other_player);
    cleanup_test_data(zappy, player);
}

// Tests for handle_look function
Test(look, handle_look_basic, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 1, NORTH);
    char command[] = "look";
    
    // Reset counters before test
    write_message_calls = 0;
    valid_message_calls = 0;
    
    int result = handle_look(player, command, zappy);
    
    cr_assert_eq(result, -1);
    // The actual functions might not call our mocks, so let's check if the function worked
    // by checking the result and that no crash occurred
    
    cleanup_test_data(zappy, player);
}

Test(look, handle_look_with_resources, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    char command[] = "look";
    
    // Add resources
    zappy->game->map->tiles[5][5].nbFood = 1;
    zappy->game->map->tiles[5][5].nbLinemate = 2;
    
    // Reset counters before test
    write_message_calls = 0;
    valid_message_calls = 0;
    
    int result = handle_look(player, command, zappy);
    
    cr_assert_eq(result, -1);
    // Test that the function returns successfully
    
    cleanup_test_data(zappy, player);
}

Test(look, handle_look_command_ignored, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    char command[] = "look extra params";
    
    // Reset counters before test
    write_message_calls = 0;
    valid_message_calls = 0;
    
    int result = handle_look(player, command, zappy);
    
    cr_assert_eq(result, -1);
    // Command parameter should be ignored as per (void)command in function
    
    cleanup_test_data(zappy, player);
}

// Additional tests to improve coverage
Test(look, append_to_message_functionality, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    
    // Add various resources to test append_to_message function
    zappy->game->map->tiles[5][5].nbFood = 1;
    zappy->game->map->tiles[5][5].nbLinemate = 1;
    zappy->game->map->tiles[5][5].nbDeraumere = 1;
    zappy->game->map->tiles[5][5].nbSibur = 1;
    zappy->game->map->tiles[5][5].nbMendiane = 1;
    zappy->game->map->tiles[5][5].nbPhiras = 1;
    zappy->game->map->tiles[5][5].nbThystame = 1;
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    // Verify all resource types are appended correctly
    cr_assert(strstr(result, "food") != NULL);
    cr_assert(strstr(result, "linemate") != NULL);
    cr_assert(strstr(result, "deraumere") != NULL);
    cr_assert(strstr(result, "sibur") != NULL);
    cr_assert(strstr(result, "mendiane") != NULL);
    cr_assert(strstr(result, "phiras") != NULL);
    cr_assert(strstr(result, "thystame") != NULL);
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, process_tile_functionality, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 1, NORTH);
    player_t *other_player = create_test_player(4, 5, 0, NORTH); // Different position
    
    // Add other player to team
    zappy->game->teams->players = other_player;
    
    // Add resources to the tile the player will look at
    zappy->game->map->tiles[4][5].nbFood = 2;
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    cleanup_test_data(zappy, player);
}

Test(look, finalize_message_functionality, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    // Check message finalization works correctly
    cr_assert(result[strlen(result) - 1] == '\n');
    cr_assert(result[strlen(result) - 2] == ']');
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, level_0_single_tile, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    // Level 0 should only see 1 tile (2*0+1 = 1)
    int comma_count = 0;
    for (char *p = result; *p; p++) {
        if (*p == ',') comma_count++;
    }
    cr_assert_eq(comma_count, 0); // No commas for single tile
    
    free(result);
    cleanup_test_data(zappy, player);
}

Test(look, multiple_players_same_tile, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    player_t *player2 = create_test_player(5, 5, 0, NORTH);
    player_t *player3 = create_test_player(5, 5, 0, NORTH);
    
    // Add multiple players to same team on same tile
    player2->next = player3;
    zappy->game->teams->players = player2;
    
    char *result = look_up(player, zappy);
    
    cr_assert_not_null(result);
    // Should contain multiple "player" mentions
    int player_count = 0;
    char *ptr = result;
    while ((ptr = strstr(ptr, "player")) != NULL) {
        player_count++;
        ptr += 6;
    }
    cr_assert_geq(player_count, 3); // At least 3 players
    
    free(result);
    free(player2->team);
    free(player2->inventory);
    free(player2->network);
    free(player3->team);
    free(player3->inventory);
    free(player3->network);
    free(player2);
    free(player3);
    cleanup_test_data(zappy, player);
}

Test(look, print_look_serve_tst, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5, 0, NORTH);
    
    int result = print_look_server(player);
    cr_assert_eq(result, 0);
    cleanup_test_data(zappy, player);
}
