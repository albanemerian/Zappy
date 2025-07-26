/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_incantation
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "network.h"
#include "buffer.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Simple buffer creation for tests if create_buffer doesn't exist
static buffer_t *test_create_buffer(void)
{
    buffer_t *buffer = malloc(sizeof(buffer_t));

    if (!buffer) {
        printf("Failed to allocate memory for buffer.\n");
        return NULL;
    }
    buffer->head = 0;
    buffer->tail = 0;
    buffer->full = 0;
    return buffer;
}

// Mock variables
static int send_start_incantation_calls = 0;
static int send_end_incantation_calls = 0;
static int send_map_tile_calls = 0;
static int send_start_incantation_return = 0;
static int send_end_incantation_return = 0;
static int send_map_tile_return = 0;


static zappy_t *create_test_zappy(int width, int height)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->params = malloc(sizeof(params_t));
    zappy->game = malloc(sizeof(game_t));
    zappy->game->map = malloc(sizeof(map_t));
    zappy->game->map->width = width;
    zappy->game->map->height = height;
    zappy->params->freq = 100;
    
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

static player_t *create_test_player(int id, int x, int y, int level, const char *team)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->inventory = malloc(sizeof(inventory_t));
    
    player->id = id;
    player->posX = x;
    player->posY = y;
    player->level = level;
    player->direction = NORTH;
    player->team = strdup(team);
    player->network->fd = 42;
    player->network->readingBuffer = test_create_buffer();
    player->network->writingBuffer = test_create_buffer();
    player->is_busy = false;
    player->remaining_cooldown = 0;
    player->current_action = NULL;
    player->next = NULL;
    
    // Initialize inventory
    memset(player->inventory, 0, sizeof(inventory_t));
    player->inventory->nbFood = 10;
    
    return player;
}

static void setup_tile_resources(zappy_t *zappy, int x, int y, int food, int linemate, 
    int deraumere, int sibur, int mendiane, int phiras, int thystame)
{
    inventory_t *tile = &zappy->game->map->tiles[x][y];
    tile->nbFood = food;
    tile->nbLinemate = linemate;
    tile->nbDeraumere = deraumere;
    tile->nbSibur = sibur;
    tile->nbMendiane = mendiane;
    tile->nbPhiras = phiras;
    tile->nbThystame = thystame;
}

static void add_player_to_team(zappy_t *zappy, player_t *player)
{
    player->next = zappy->game->teams->players;
    zappy->game->teams->players = player;
}

static void cleanup_test_data(zappy_t *zappy, player_t *player)
{
    if (zappy) {
        // Free players in teams
        if (zappy->game && zappy->game->teams) {
            player_t *current = zappy->game->teams->players;
            while (current) {
                player_t *next = current->next;
                if (current != player) { // Don't double free
                    free(current->team);
                    free(current->inventory);
                    free(current->network);
                    if (current->current_action)
                        free(current->current_action);
                    free(current);
                }
                current = next;
            }
            free(zappy->game->teams->name);
            free(zappy->game->teams);
        }
        
        if (zappy->game && zappy->game->map) {
            for (int i = 0; i < zappy->game->map->height; i++) {
                free(zappy->game->map->tiles[i]);
            }
            free(zappy->game->map->tiles);
            free(zappy->game->map);
        }
        if (zappy->game) free(zappy->game);
        if (zappy->params) free(zappy->params);
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player->inventory);
        free(player->network);
        if (player->current_action)
            free(player->current_action);
        free(player);
    }
    
    // Reset mock counters
    send_start_incantation_calls = 0;
    send_end_incantation_calls = 0;
    send_map_tile_calls = 0;
    send_start_incantation_return = 0;
    send_end_incantation_return = 0;
    send_map_tile_return = 0;
}

// Tests for check_player_on_tile function
Test(incantation, check_player_on_tile_level_1_to_2_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(1, 5, 5, 1, "team1");
    
    // Level 1->2 requires: 1 player, 1 linemate
    setup_tile_resources(zappy, 5, 5, 0, 1, 0, 0, 0, 0, 0);
    add_player_to_team(zappy, player);
    
    int result = check_player_on_tile(player, zappy);
    
    cr_assert_eq(result, 1); // 1 player on tile
    
    cleanup_test_data(zappy, player);
}

Test(incantation, check_player_on_tile_level_2_to_3_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player1 = create_test_player(1, 5, 5, 2, "team1");
    player_t *player2 = create_test_player(2, 5, 5, 2, "team1");
    
    // Level 2->3 requires: 2 players, 1 linemate, 1 deraumere, 1 sibur
    setup_tile_resources(zappy, 5, 5, 0, 1, 1, 1, 0, 0, 0);
    add_player_to_team(zappy, player1);
    add_player_to_team(zappy, player2);
    
    int result = check_player_on_tile(player1, zappy);
    
    cr_assert_eq(result, 2); // 2 players on tile
    
    cleanup_test_data(zappy, NULL); // player1 and player2 freed in teams cleanup
}

Test(incantation, check_player_on_tile_insufficient_players, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(1, 5, 5, 2, "team1");
    
    // Level 2->3 requires 2 players, but only 1 available
    setup_tile_resources(zappy, 5, 5, 0, 1, 1, 1, 0, 0, 0);
    add_player_to_team(zappy, player);
    
    int result = check_player_on_tile(player, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, NULL);
}

Test(incantation, check_player_on_tile_insufficient_resources, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(1, 5, 5, 1, "team1");
    
    // Level 1->2 requires 1 linemate, but tile has none
    setup_tile_resources(zappy, 5, 5, 0, 0, 0, 0, 0, 0, 0);
    add_player_to_team(zappy, player);
    
    int result = check_player_on_tile(player, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, NULL);
}

Test(incantation, check_player_on_tile_max_level, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(1, 5, 5, 8, "team1");
    
    // Level 8 is max, can't elevate further
    add_player_to_team(zappy, player);
    
    int result = check_player_on_tile(player, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, NULL);
}

// Tests for mark_players_incanting function
Test(incantation, mark_players_incanting_single_player, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(1, 5, 5, 1, "team1");
    add_player_to_team(zappy, player);
    
    int player_list[] = {1};
    
    mark_players_incanting(player_list, 1, zappy);
    
    cr_assert_eq(player->is_busy, true);
    cr_assert_neq(player->remaining_cooldown, 0);
    cr_assert_str_eq(player->current_action, "Incantation");
    
    cleanup_test_data(zappy, NULL);
}

Test(incantation, mark_players_incanting_multiple_players, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player1 = create_test_player(1, 5, 5, 2, "team1");
    player_t *player2 = create_test_player(2, 5, 5, 2, "team1");
    player_t *player3 = create_test_player(3, 7, 7, 2, "team1"); // Different position
    
    add_player_to_team(zappy, player1);
    add_player_to_team(zappy, player2);
    add_player_to_team(zappy, player3);
    
    int player_list[] = {1, 2}; // Only players 1 and 2 in incantation
    
    mark_players_incanting(player_list, 2, zappy);
    
    cr_assert_eq(player1->is_busy, true);
    cr_assert_eq(player2->is_busy, true);
    cr_assert_eq(player3->is_busy, false); // Not in incantation
    
    cleanup_test_data(zappy, NULL);
}

Test(incantation, remove_crystal_from_tiles_invalid_level, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    setup_tile_resources(zappy, 5, 5, 0, 2, 2, 2, 2, 2, 2);
    
    remove_crystal_from_tiles(5, 5, 8, zappy); // Level 8->9 doesn't exist
    
    // Resources should remain unchanged
    cr_assert_eq(zappy->game->map->tiles[5][5].nbLinemate, 2);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbDeraumere, 2);
    
    cleanup_test_data(zappy, NULL);
}

// Tests for validate_and_get_players function
Test(incantation, validate_and_get_players_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(1, 5, 5, 1, "team1");
    
    setup_tile_resources(zappy, 5, 5, 0, 1, 0, 0, 0, 0, 0);
    add_player_to_team(zappy, player);
    
    int *player_list = NULL;
    int result = validate_and_get_players(player, zappy, &player_list);
    
    cr_assert_eq(result, 1);
    cr_assert_not_null(player_list);
    cr_assert_eq(player_list[0], 1);
    
    free(player_list);
    cleanup_test_data(zappy, NULL);
}

Test(incantation, validate_and_get_players_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(1, 5, 5, 1, "team1");
    
    // No resources for incantation
    setup_tile_resources(zappy, 5, 5, 0, 0, 0, 0, 0, 0, 0);
    add_player_to_team(zappy, player);
    
    int *player_list = NULL;
    int result = validate_and_get_players(player, zappy, &player_list);
    
    cr_assert_eq(result, -1);
    cr_assert_null(player_list);
    
    cleanup_test_data(zappy, NULL);
}


Test(incantation, player_current_action_cleanup, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(1, 5, 5, 1, "team1");
    
    // Set existing action
    player->current_action = strdup("forward");
    
    add_player_to_team(zappy, player);
    int player_list[] = {1};
    
    mark_players_incanting(player_list, 1, zappy);
    
    cr_assert_str_eq(player->current_action, "Incantation");
    
    cleanup_test_data(zappy, NULL);
}
