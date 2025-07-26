/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_freePlayer
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

// Forward declarations of functions to test
void free_networkplayer(player_t *player);
void free_players(player_t *player);
void free_teams(team_t *teams);
void *free_params(params_t *params);
void free_action_queue_pl(action_queue_t *queue);
void free_map(map_t *map);
void *free_zappy(zappy_t *zappy);
void *free_player(player_t *player);

Test(free_networkplayer, frees_network_fields, .init=redirect_all_std)
{
    player_t *player = calloc(1, sizeof(player_t));
    player->network = calloc(1, sizeof(network_t));
    player->network->readingBuffer = create_buffer('\n');
    player->network->writingBuffer = create_buffer('\n');

    free_networkplayer(player);

    // No crash = success
    free(player);
}

Test(free_players, frees_single_player, .init=redirect_all_std)
{
    player_t *player = calloc(1, sizeof(player_t));
    player->inventory = calloc(1, sizeof(inventory_t));
    player->network = calloc(1, sizeof(network_t));
    player->network->readingBuffer = create_buffer('\n');
    player->network->writingBuffer = create_buffer('\n');
    player->pending_actions = NULL;
    player->team = strdup("team");
    player->current_action = strdup("action");

    free_players(player);

    // No crash = success
}

Test(free_players, frees_multiple_players, .init=redirect_all_std)
{
    player_t *p1 = calloc(1, sizeof(player_t));
    player_t *p2 = calloc(1, sizeof(player_t));
    p1->next = p2;
    p1->inventory = calloc(1, sizeof(inventory_t));
    p2->inventory = calloc(1, sizeof(inventory_t));
    p1->team = strdup("team1");
    p2->team = strdup("team2");

    free_players(p1);

    // No crash = success
}

Test(free_teams, frees_single_team, .init=redirect_all_std)
{
    team_t *team = calloc(1, sizeof(team_t));
    team->name = strdup("teamA");
    team->players = NULL;

    free_teams(team);

    // No crash = success
}

Test(free_teams, frees_team_with_players, .init=redirect_all_std)
{
    team_t *team = calloc(1, sizeof(team_t));
    team->name = strdup("teamB");
    player_t *player = calloc(1, sizeof(player_t));
    player->inventory = calloc(1, sizeof(inventory_t));
    team->players = player;

    free_teams(team);

    // No crash = success
}

Test(free_teams, frees_multiple_teams, .init=redirect_all_std)
{
    team_t *t1 = calloc(1, sizeof(team_t));
    team_t *t2 = calloc(1, sizeof(team_t));
    t1->next = t2;
    t1->name = strdup("team1");
    t2->name = strdup("team2");

    free_teams(t1);

    // No crash = success
}

Test(free_params, frees_params_with_teams, .init=redirect_all_std)
{
    params_t *params = calloc(1, sizeof(params_t));
    params->nb_team = 2;
    params->teams = calloc(2, sizeof(char *));
    params->teams[0] = strdup("team1");
    params->teams[1] = strdup("team2");

    free_params(params);

    // No crash = success
}

Test(free_params, frees_params_without_teams, .init=redirect_all_std)
{
    params_t *params = calloc(1, sizeof(params_t));
    params->nb_team = 0;
    params->teams = NULL;

    free_params(params);

    // No crash = success
}

Test(free_action_queue_pl, frees_action_queue, .init=redirect_all_std)
{
    action_queue_t *queue = calloc(1, sizeof(action_queue_t));
    action_request_t *req1 = calloc(1, sizeof(action_request_t));
    action_request_t *req2 = calloc(1, sizeof(action_request_t));
    req1->command = strdup("cmd1");
    req1->next = req2;
    req2->command = strdup("cmd2");
    queue->head = req1;

    free_action_queue_pl(queue);

    // No crash = success
}

Test(free_map, frees_map_with_eggs, .init=redirect_all_std)
{
    map_t *map = calloc(1, sizeof(map_t));
    map->height = 2;
    map->tiles = calloc(2, sizeof(inventory_t *));
    map->tiles[0] = calloc(1, sizeof(inventory_t));
    map->tiles[1] = calloc(1, sizeof(inventory_t));
    egg_t *egg = calloc(1, sizeof(egg_t));
    egg->teamName = strdup("eggteam");
    map->currentEggs = egg;

    free_map(map);

    // No crash = success
}

Test(free_zappy, frees_zappy_struct, .init=redirect_all_std)
{
    zappy_t *zappy = calloc(1, sizeof(zappy_t));
    zappy->params = calloc(1, sizeof(params_t));
    zappy->network = calloc(1, sizeof(network_t));
    zappy->network->sockfd = -1;
    zappy->game = calloc(1, sizeof(game_t));
    zappy->graph = calloc(1, sizeof(graph_net_t));
    zappy->graph->network = calloc(1, sizeof(network_t));
    zappy->graph->network->readingBuffer = create_buffer('\n');
    zappy->graph->network->writingBuffer = create_buffer('\n');
    zappy->unified_poll = calloc(1, sizeof(unified_poll_t));
    zappy->unified_poll->fds = malloc(10 * sizeof(struct pollfd));

    free_zappy(zappy);

    // No crash = success
}

Test(free_player, frees_player_with_inventory, .init=redirect_all_std)
{
    player_t *player = calloc(1, sizeof(player_t));
    player->inventory = calloc(1, sizeof(inventory_t));

    free_player(player);

    // No crash = success
}

Test(free_player, frees_null_player, .init=redirect_all_std)
{
    player_t *player = NULL;

    free_player(player);

    // No crash = success
}
