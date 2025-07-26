/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** get_objects
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

// Function declarations
int remove_element_from_tile(zappy_t *zappy, int x, int y, char *name);
int handle_take(player_t *player, char *command, zappy_t *zappy);
int send_map_tile(inventory_t **tiles, zappy_t *zappy, int posX, int posY);
int send_player_inventory(zappy_t *zappy, player_t *player);
int send_ressource_collected(zappy_t *zappy, player_t *player, int ressourceType);

// Mock variables
static int send_map_tile_calls = 0;
static int send_player_inventory_calls = 0;
static int send_ressource_collected_calls = 0;
static int send_map_tile_return = 0;
static int send_player_inventory_return = 0;
static int send_ressource_collected_return = 0;


// Helper functions
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
    
    return zappy;
}

static player_t *create_test_player(int x, int y)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->inventory = malloc(sizeof(inventory_t));
    
    player->id = 1;
    player->posX = x;
    player->posY = y;
    player->level = 1;
    player->direction = NORTH;
    player->team = strdup("team1");
    player->network->fd = 42;
    player->next = NULL;
    
    // Initialize inventory with zeros
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
        free(zappy->game);
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player->inventory);
        free(player->network);
        free(player);
    }
    // Reset mock counters
    send_map_tile_calls = 0;
    send_player_inventory_calls = 0;
    send_ressource_collected_calls = 0;
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_collected_return = 0;
}

// Tests for remove_element_from_tile function
Test(take_object, remove_element_from_tile_food_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    // Add food to tile
    zappy->game->map->tiles[5][5].nbFood = 3;
    
    int result = remove_element_from_tile(zappy, 5, 5, "food");
    
    cr_assert_eq(result, 0);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbFood, 2);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_food_empty, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    // No food on tile (default 0)
    int result = remove_element_from_tile(zappy, 5, 5, "food");
    
    cr_assert_eq(result, -1);
    cr_assert_eq(zappy->game->map->tiles[5][5].nbFood, 0);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_linemate_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    zappy->game->map->tiles[3][7].nbLinemate = 2;
    
    int result = remove_element_from_tile(zappy, 3, 7, "linemate");
    
    cr_assert_eq(result, 0);
    cr_assert_eq(zappy->game->map->tiles[3][7].nbLinemate, 1);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_deraumere_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    zappy->game->map->tiles[2][8].nbDeraumere = 1;
    
    int result = remove_element_from_tile(zappy, 2, 8, "deraumere");
    
    cr_assert_eq(result, 0);
    cr_assert_eq(zappy->game->map->tiles[2][8].nbDeraumere, 0);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_sibur_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    zappy->game->map->tiles[1][9].nbSibur = 4;
    
    int result = remove_element_from_tile(zappy, 1, 9, "sibur");
    
    cr_assert_eq(result, 0);
    cr_assert_eq(zappy->game->map->tiles[1][9].nbSibur, 3);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_mendiane_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    zappy->game->map->tiles[6][4].nbMendiane = 1;
    
    int result = remove_element_from_tile(zappy, 6, 4, "mendiane");
    
    cr_assert_eq(result, 0);
    cr_assert_eq(zappy->game->map->tiles[6][4].nbMendiane, 0);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_phiras_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    zappy->game->map->tiles[8][2].nbPhiras = 2;
    
    int result = remove_element_from_tile(zappy, 8, 2, "phiras");
    
    cr_assert_eq(result, 0);
    cr_assert_eq(zappy->game->map->tiles[8][2].nbPhiras, 1);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_thystame_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    zappy->game->map->tiles[9][1].nbThystame = 3;
    
    int result = remove_element_from_tile(zappy, 9, 1, "thystame");
    
    cr_assert_eq(result, 0);
    cr_assert_eq(zappy->game->map->tiles[9][1].nbThystame, 2);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_invalid_resource, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    int result = remove_element_from_tile(zappy, 5, 5, "invalid");
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, remove_element_from_tile_empty_resources, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    
    // Test all resources when tile is empty
    char *resources[] = {"food", "linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    
    for (int i = 0; i < 7; i++) {
        int result = remove_element_from_tile(zappy, 5, 5, resources[i]);
        cr_assert_eq(result, -1);
    }
    
    cleanup_test_data(zappy, NULL);
}

Test(take_object, handle_take_no_command_parameter, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "take";
    
    int result = handle_take(player, command, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(take_object, handle_take_no_resource_parameter, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "take ";
    
    int result = handle_take(player, command, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(take_object, handle_take_empty_command, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "";
    
    int result = handle_take(player, command, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(take_object, handle_take_resource_not_on_tile, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "take food";
    
    // Tile has no food (default 0)
    int result = handle_take(player, command, zappy);
    
    cr_assert_eq(result, -1);
    cr_assert_eq(player->inventory->nbFood, 0);
    
    cleanup_test_data(zappy, player);
}

Test(take_object, handle_take_invalid_resource, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    char command[] = "take invalid_resource";
    
    int result = handle_take(player, command, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}


Test(take_object, add_item_to_inventory_null_parameters, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    
    // Test with NULL inventory
    free(player->inventory);
    player->inventory = NULL;
    char command[] = "take food";
    
    zappy->game->map->tiles[5][5].nbFood = 1;
    
    int result = handle_take(player, command, zappy);
    cr_assert_eq(result, -1);
    
    // Manual cleanup due to NULL inventory
    if (zappy) {
        for (int i = 0; i < zappy->game->map->height; i++) {
            free(zappy->game->map->tiles[i]);
        }
        free(zappy->game->map->tiles);
        free(zappy->game->map);
        free(zappy->game);
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player->network);
        free(player);
    }
}

Test(take_object, take_unknown_resource_type, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy(10, 10);
    player_t *player = create_test_player(5, 5);
    
    send_map_tile_return = 0;
    send_player_inventory_return = 0;
    send_ressource_collected_return = 0;
    
    // Test with unknown resource (should use default type 0)
    char command[] = "take unknown";
    
    int result = handle_take(player, command, zappy);
    cr_assert_eq(result, -1); // Should fail at remove_element_from_tile
    
    cleanup_test_data(zappy, player);
}
