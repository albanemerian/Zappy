/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_team
*/

#include "zappy.h"
#include "game.h"
#include "network.h"

#include <criterion/criterion.h>
#include <criterion/redirect.h>


static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(food_refill, refill_food, .init = redirect_all_std)
{

    refill_food(NULL);
}

Test(game_null, refill_food_null_game, .init = redirect_all_std)
{
    zappy_t zappy = {0};

    zappy.game = NULL;
    refill_food(&zappy);
}

Test(game_map_null, refill_food_null_map, .init = redirect_all_std)
{
    zappy_t zappy = {0};

    zappy.game = malloc(sizeof(game_t));
    zappy.game->map = NULL;
    refill_food(&zappy);
    free(zappy.game);
}

Test(game_map_tiles_null, refill_food_null_tiles, .init = redirect_all_std)
{
    zappy_t zappy = {0};

    zappy.game = malloc(sizeof(game_t));
    zappy.game->map = malloc(sizeof(map_t));
    zappy.game->map->tiles = NULL;
    refill_food(&zappy);
    free(zappy.game->map);
    free(zappy.game);
}

static zappy_t *create_test_server(int width, int height, bool debug)
{
    zappy_t *server = malloc(sizeof(zappy_t));
    server->params = malloc(sizeof(params_t));
    server->params->x = width;
    server->params->y = height;
    server->params->is_debug = debug;
    server->game = NULL;
    return server;
}


Test(game_map_tiles_valid, refill_food_valid_tiles, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_server(5, 5, false);

    refill_food(zappy);
}
