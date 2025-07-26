/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_free
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "algo.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test free_params with NULL
Test(free_params, test_null_params, .init = redirect_all_std)
{
    void *result = free_params(NULL);
    cr_assert_null(result);
}

// Test free_params with valid params but no teams
Test(free_params, test_params_no_teams, .init = redirect_all_std)
{
    params_t *params = malloc(sizeof(params_t));
    params->teams = NULL;
    params->nb_team = 0;
    
    void *result = free_params(params);
    cr_assert_null(result);
}

// Test free_params with valid params and teams
Test(free_params, test_params_with_teams, .init = redirect_all_std)
{
    params_t *params = malloc(sizeof(params_t));
    params->nb_team = 2;
    params->teams = malloc(sizeof(char*) * 2);
    params->teams[0] = malloc(10);
    params->teams[1] = malloc(10);
    
    void *result = free_params(params);
    cr_assert_null(result);
}

// Test free_server with NULL
Test(free_server, test_null_server, .init = redirect_all_std)
{
    void *result = free_zappy(NULL);
    cr_assert_null(result);
}

Test(free_functions, free_params_with_teams, .init = redirect_all_std)
{
    params_t *params = malloc(sizeof(params_t));
    params->teams = malloc(sizeof(char*) * 3);
    params->teams[0] = strdup("team1");
    params->teams[1] = strdup("team2");
    params->teams[2] = NULL;
    params->nb_team = 2;
    
    void *result = free_params(params);
    
    cr_assert_null(result);
}

Test(free_functions, free_params_null, .init = redirect_all_std)
{
    void *result = free_params(NULL);
    
    cr_assert_null(result);
}

Test(free_functions, free_player_basic, .init = redirect_all_std)
{
    player_t *player = malloc(sizeof(player_t));
    player->id = 1;
    player->level = 1;
    player->posX = 0;
    player->posY = 0;
    player->direction = NORTH;
    player->inventory = malloc(sizeof(inventory_t));
    player->network = malloc(sizeof(network_t));
    player->network->fd = -1;
    player->next = NULL;
    
    void *result = free_player(player);
    
    cr_assert_null(result);
}

Test(free_functions, free_player_null, .init = redirect_all_std)
{
    void *result = free_player(NULL);
    
    cr_assert_null(result);
}

Test(free_functions, free_map_basic, .init = redirect_all_std)
{
    map_t *map = malloc(sizeof(map_t));
    map->width = 3;
    map->height = 3;
    map->tiles = malloc(sizeof(inventory_t*) * 3);
    
    for (int i = 0; i < 3; i++) {
        map->tiles[i] = malloc(sizeof(inventory_t) * 3);
    }
    
    free_map(map);
    // Test passes if no segfault occurs
    cr_assert(true);
}

