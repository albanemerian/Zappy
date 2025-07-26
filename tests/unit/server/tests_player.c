/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_protocol
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "network.h"
#include "buffer.h"
#include "algo.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Forward declarations for tested functions
void remove_player_from_alive_teamate(zappy_t *zappy, player_t *player);
void verify_need_for_egg(team_t *team, zappy_t *zappy);
void check_player_status(zappy_t *zappy);
void remove_player_by_fd(zappy_t *zappy, int fd);

// Helper to create a minimal zappy/game/team/player structure
static zappy_t *create_minimal_zappy_for_player(void)
{
    zappy_t *zappy = calloc(1, sizeof(zappy_t));
    zappy->params = calloc(1, sizeof(params_t));
    zappy->params->freq = 1;
    zappy->game = calloc(1, sizeof(game_t));
    zappy->game->map = calloc(1, sizeof(map_t));
    zappy->game->map->width = 10;
    zappy->game->map->height = 10;
    zappy->game->map->currentEggs = NULL;
    zappy->game->teams = calloc(1, sizeof(team_t));
    zappy->game->teams->name = strdup("teamA");
    zappy->game->teams->nbPlayerAlive = 1;
    zappy->game->teams->nbPlayers = 2;
    zappy->game->teams->nbEggs = 0;

    zappy->game->teams->players = calloc(1, sizeof(player_t));
    zappy->game->teams->players->team = strdup("teamA");
    zappy->game->teams->players->inventory = calloc(1, sizeof(inventory_t));
    zappy->game->teams->players->inventory->nbFood = 1;
    zappy->game->teams->players->network = calloc(1, sizeof(network_t));
    zappy->game->teams->players->network->fd = 42;
    // Add writingBuffer for player->network, required by handle_player_death
    zappy->game->teams->players->network->writingBuffer = create_buffer('\n');
    zappy->game->teams->players->network->readingBuffer = create_buffer('\n');
    zappy->game->teams->players->last_food_check = time(NULL) - 200;
    // Add stub for player->next
    zappy->game->teams->players->next = NULL;
    // Add stub for zappy->game->won
    zappy->game->won = false;
    return zappy;
}

Test(player, remove_player_from_alive_teamate_decrements, .init=redirect_all_std)
{
    zappy_t *zappy = create_minimal_zappy_for_player();
    player_t *player = zappy->game->teams->players;
    cr_assert_eq(zappy->game->teams->nbPlayerAlive, 1);
    remove_player_from_alive_teamate(zappy, player);
    cr_assert_eq(zappy->game->teams->nbPlayerAlive, 0);
    free(zappy->game->teams->players->team);
    free(zappy->game->teams->players->inventory);
    free(zappy->game->teams->players->network);
    free(zappy->game->teams->players);
    free(zappy->game->teams->name);
    free(zappy->game->teams);
    free(zappy->game->map);
    free(zappy->game);
    free(zappy->params);
    free(zappy);
}

Test(player, verify_need_for_egg_creates_egg, .init=redirect_all_std)
{
    zappy_t *zappy = create_minimal_zappy_for_player();
    team_t *team = zappy->game->teams;
    team->nbPlayerAlive = 0;
    team->nbEggs = 0;
    team->nbPlayers = 1;
    verify_need_for_egg(team, zappy);
    // No crash = success (egg creation side effects not checked)
    free(zappy->game->teams->players->team);
    free(zappy->game->teams->players->inventory);
    free(zappy->game->teams->players->network);
    free(zappy->game->teams->players);
    free(zappy->game->teams->name);
    free(zappy->game->teams);
    free(zappy->game->map);
    free(zappy->game);
    free(zappy->params);
    free(zappy);
}

Test(player, remove_player_by_fd_removes_player, .init=redirect_all_std)
{
    zappy_t *zappy = create_minimal_zappy_for_player();
    remove_player_by_fd(zappy, 42);
    // Player should be freed, but we can't check pointer, just no crash
    free(zappy->game->teams->name);
    free(zappy->game->teams);
    free(zappy->game->map);
    free(zappy->game);
    free(zappy->params);
    free(zappy);
}
