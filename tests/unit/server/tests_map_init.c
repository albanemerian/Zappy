/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** test_game_init
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>
#include <unistd.h>

#include "zappy.h"
#include "algo.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Helper function to create test server
static zappy_t *create_test_server(int width, int height, bool debug)
{
    zappy_t *server = malloc(sizeof(zappy_t));
    server->params = malloc(sizeof(params_t));
    server->params->x = width;
    server->params->y = height;
    server->params->is_debug = debug;
    server->game = NULL;
    server->graph = malloc(sizeof(graph_net_t));
    server->graph->network = malloc(sizeof(network_t));
    server->graph->network->fd = -1; // Initialize fd to -1
    return server;
}

// Helper function to free test server

// Test shuffle_fisher function
Test(algo, shuffle_fisher_basic, .init = redirect_all_std)
{
    tiles_t *tiles = shuffle_fisher(3, 3);
    
    cr_assert_not_null(tiles, "shuffle_fisher should return non-null array");
    
    // Check that all coordinates are valid
    for (int i = 0; i < 9; i++) {
        cr_assert_geq(tiles[i].x, 0, "x coordinate should be >= 0");
        cr_assert_lt(tiles[i].x, 3, "x coordinate should be < width");
        cr_assert_geq(tiles[i].y, 0, "y coordinate should be >= 0");
        cr_assert_lt(tiles[i].y, 3, "y coordinate should be < height");
    }
    
    free(tiles);
}

Test(algo, shuffle_fisher_coverage, .init = redirect_all_std)
{
    tiles_t *tiles = shuffle_fisher(2, 2);
    bool found[4] = {false, false, false, false};
    
    // Check that all expected tiles are present
    for (int i = 0; i < 4; i++) {
        int index = tiles[i].y * 2 + tiles[i].x;
        cr_assert_geq(index, 0, "Tile index should be >= 0");
        cr_assert_lt(index, 4, "Tile index should be < total");
        found[index] = true;
    }
    
    // Verify all tiles are present
    for (int i = 0; i < 4; i++) {
        cr_assert(found[i], "All tiles should be present in shuffled array");
    }
    
    free(tiles);
}

Test(game_init, init_game_basic, .init = redirect_all_std)
{
    zappy_t *server = create_test_server(5, 5, false);
    
    init_game(server);
    
    cr_assert_not_null(server->game, "game should be initialized");
    cr_assert_eq(server->game->map->width, 5, "game width should be set correctly");
    cr_assert_eq(server->game->map->height, 5, "game height should be set correctly");
    cr_assert_null(server->game->teams, "Teams should be null initially");
}
