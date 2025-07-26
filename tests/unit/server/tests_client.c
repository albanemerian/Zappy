/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_client
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "zappy.h"



static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Mock functions and helper structures
static zappy_t *create_mock_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->params = malloc(sizeof(params_t));
    zappy->graph = malloc(sizeof(graph_net_t));
    zappy->game = malloc(sizeof(game_t));
    zappy->game->map = malloc(sizeof(map_t));
    zappy->game->map->currentEggs = malloc(sizeof(egg_t));
    zappy->game->teams = NULL;
    
    zappy->params->nb_team = 2;
    zappy->params->nb_client = 5;
    zappy->params->teams = malloc(sizeof(char*) * 2);
    zappy->params->teams[0] = strdup("team1");
    zappy->params->teams[1] = strdup("team2");
    
    zappy->graph->network = malloc(sizeof(network_t));
    zappy->graph->network->fd = -1;
    zappy->game->map->currentEggs = NULL;
    
    return zappy;
}

static void free_mock_zappy(zappy_t *zappy)
{
    if (!zappy) return;
    if (zappy->params) {
        if (zappy->params->teams) {
            for (int i = 0; i < zappy->params->nb_team; i++) {
                free(zappy->params->teams[i]);
            }
            free(zappy->params->teams);
        }
        free(zappy->params);
    }
    if (zappy->graph) free(zappy->graph);
    if (zappy->game) {
        if (zappy->game->map) free(zappy->game->map);
        free(zappy->game);
    }
    free(zappy);
}

static team_t *create_mock_team(const char *name, int nb_players)
{
    team_t *team = malloc(sizeof(team_t));
    team->name = strdup(name);
    team->nbPlayers = nb_players;
    team->nbPlayerAlive = nb_players;
    team->players = NULL;
    team->next = NULL;
    return team;
}

// Tests for process_new_client function
Test(process_new_client, graphic_client_success, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    int fd = 42;
    
    bool result = process_new_client("GRAPHIC", fd, zappy);
    
    cr_assert_eq(result, true);
    cr_assert_eq(zappy->graph->network->fd, fd);
    
    free_mock_zappy(zappy);
}

Test(process_new_client, graphic_client_already_connected, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    zappy->graph->network->fd = 10; // Already connected
    int fd = 42;
    
    cr_redirect_stderr();
    bool result = process_new_client("GRAPHIC", fd, zappy);
    
    cr_assert_eq(result, true);
    free_mock_zappy(zappy);
}

Test(process_new_client, valid_team_name, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    
    bool result = process_new_client("team1", 42, zappy);
    
    cr_assert_eq(result, true);
    
    free_mock_zappy(zappy);
}

Test(process_new_client, invalid_team_name, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    
    cr_redirect_stderr();
    bool result = process_new_client("invalid_team", 42, zappy);
    
    cr_assert_eq(result, false);
    
    free_mock_zappy(zappy);
}

// Tests for add_client_to_team function
Test(add_client_to_team, graphic_client_returns_null, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    
    team_t *result = add_client_to_team("GRAPHIC", 42, zappy);
    
    cr_assert_null(result);
    
    free_mock_zappy(zappy);
}

Test(add_client_to_team, successful_addition_to_team, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    team_t *team = create_mock_team("team1", 2);
    zappy->game->teams = team;
    
    team_t *result = add_client_to_team("team1", 42, zappy);
    
    cr_assert_not_null(result);
    cr_assert_eq(team->nbPlayers, 3);
    cr_assert_eq(team->nbPlayerAlive, 3);
    cr_assert_not_null(team->players);
    cr_assert_eq(team->players->network->fd, 42);
    cr_assert_str_eq(team->players->team, "team1");
    
    // Cleanup
    if (team->players) {
        free(team->players->team);
        free(team->players->inventory);
        free(team->players->network->writingBuffer);
        free(team->players->network);
        free(team->players);
    }
    free(team->name);
    free(team);
    free_mock_zappy(zappy);
}

Test(add_client_to_team, team_at_capacity, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    team_t *team = create_mock_team("team1", 5); // At max capacity
    zappy->game->teams = team;
    
    team_t *result = add_client_to_team("team1", 42, zappy);
    
    cr_assert_null(result);
    cr_assert_eq(team->nbPlayers, 5); // Should not change
    
    free(team->name);
    free(team);
    free_mock_zappy(zappy);
}

Test(add_client_to_team, nonexistent_team, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    team_t *team = create_mock_team("team1", 2);
    zappy->game->teams = team;
    
    team_t *result = add_client_to_team("nonexistent", 42, zappy);
    
    cr_assert_null(result);
    
    free(team->name);
    free(team);
    free_mock_zappy(zappy);
}

// Test for player initialization with eggs
Test(add_client_to_team, player_with_available_egg, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    team_t *team = create_mock_team("team1", 1);
    zappy->game->teams = team;
    
    // Create mock egg
    egg_t *egg = malloc(sizeof(egg_t));
    egg->posX = 10;
    egg->posY = 20;
    egg->isHatched = false;
    egg->next = NULL;
    zappy->game->map->currentEggs = egg;
    
    team_t *result = add_client_to_team("team1", 42, zappy);
    
    cr_assert_not_null(result);
    cr_assert_eq(team->players->posX, 10);
    cr_assert_eq(team->players->posY, 20);
    cr_assert_eq(egg->isHatched, true);
    
    // Cleanup
    if (team->players) {
        free(team->players->team);
        free(team->players->inventory);
        free(team->players->network->readingBuffer);
        free(team->players->network);
        free(team->players);
    }
    free(team->name);
    free(team);
    free(egg);
    free_mock_zappy(zappy);
}

Test(add_client_to_team, player_with_hatched_egg, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    team_t *team = create_mock_team("team1", 1);
    zappy->game->teams = team;
    
    // Create mock hatched egg
    egg_t *egg = malloc(sizeof(egg_t));
    egg->posX = 10;
    egg->posY = 20;
    egg->isHatched = true; // Already hatched
    egg->next = NULL;
    zappy->game->map->currentEggs = egg;
    
    team_t *result = add_client_to_team("team1", 42, zappy);
    
    cr_assert_not_null(result);
    // Player position should not be set from hatched egg
    
    // Cleanup
    if (team->players) {
        free(team->players->team);
        free(team->players->inventory);
        free(team->players->network->readingBuffer);
        free(team->players->network);
        free(team->players);
    }
    free(team->name);
    free(team);
    free(egg);
    free_mock_zappy(zappy);
}

// Test player inventory initialization
Test(add_client_to_team, player_inventory_initialized, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    team_t *team = create_mock_team("team1", 1);
    zappy->game->teams = team;
    
    team_t *result = add_client_to_team("team1", 42, zappy);
    
    cr_assert_not_null(result);
    cr_assert_not_null(team->players->inventory);
    cr_assert_eq(team->players->inventory->nbLinemate, 0);
    cr_assert_eq(team->players->inventory->nbDeraumere, 0);
    cr_assert_eq(team->players->inventory->nbSibur, 0);
    cr_assert_eq(team->players->inventory->nbMendiane, 0);
    cr_assert_eq(team->players->inventory->nbPhiras, 0);
    cr_assert_eq(team->players->inventory->nbThystame, 0);
    cr_assert_eq(team->players->level, 1);
    // cr_assert_ge(team->players->direction, 0);
    cr_assert_lt(team->players->direction, 5);
    
    // Cleanup
    if (team->players) {
        free(team->players->team);
        free(team->players->inventory);
        free(team->players->network->readingBuffer);
        free(team->players->network);
        free(team->players);
    }
    free(team->name);
    free(team);
    free_mock_zappy(zappy);
}

// Test multiple eggs scenario
Test(add_client_to_team, multiple_eggs_first_available, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    team_t *team = create_mock_team("team1", 1);
    zappy->game->teams = team;
    
    // Create multiple eggs, first one hatched
    egg_t *egg1 = malloc(sizeof(egg_t));
    egg1->posX = 10;
    egg1->posY = 20;
    egg1->isHatched = true;
    
    egg_t *egg2 = malloc(sizeof(egg_t));
    egg2->posX = 30;
    egg2->posY = 40;
    egg2->isHatched = false;
    egg2->next = NULL;
    
    egg1->next = egg2;
    zappy->game->map->currentEggs = egg1;
    
    team_t *result = add_client_to_team("team1", 42, zappy);
    
    cr_assert_not_null(result);
    cr_assert_eq(team->players->posX, 30);
    cr_assert_eq(team->players->posY, 40);
    cr_assert_eq(egg2->isHatched, true);
    
    // Cleanup
    if (team->players) {
        free(team->players->team);
        free(team->players->inventory);
        free(team->players->network->readingBuffer);
        free(team->players->network);
        free(team->players);
    }
    free(team->name);
    free(team);
    free(egg1);
    free(egg2);
    free_mock_zappy(zappy);
}

Test(add_client_to_team, multiple_eggs_all_hatched, .init = redirect_all_std)
{
    zappy_t *zappy = create_mock_zappy();
    team_t *team = create_mock_team("team1", 1);
    zappy->game->teams = team;
    
    // Create multiple eggs, all hatched
    egg_t *egg1 = malloc(sizeof(egg_t));
    egg1->posX = 10;
    egg1->posY = 20;
    egg1->isHatched = true;
    
    egg_t *egg2 = malloc(sizeof(egg_t));
    egg2->posX = 30;
    egg2->posY = 40;
    egg2->isHatched = true;
    egg2->next = NULL;
    
    egg1->next = egg2;
    zappy->game->map->currentEggs = egg1;
    
    team_t *result = add_client_to_team("team1", 42, zappy);
    
    cr_assert_not_null(result);
    // Player position should not be set from hatched eggs
    
    // Cleanup
    if (team->players) {
        free(team->players->team);
        free(team->players->inventory);
        free(team->players->network->readingBuffer);
        free(team->players->network);
        free(team->players);
    }
    free(team->name);
    free(team);
    free(egg1);
    free(egg2);
    free_mock_zappy(zappy);
}
