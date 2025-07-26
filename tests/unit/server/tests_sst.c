/*
** EPITECH PROJECT, 2025
** Tests
** File description:
** sst
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Setup function to initialize the zappy structure

static zappy_t *new_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    if (!zappy)
        return NULL;

    zappy->params = malloc(sizeof(params_t));
    if (!zappy->params) {
        free(zappy);
        return NULL;
    }
    zappy->params->port = 4242;
    zappy->params->x = 10;
    zappy->params->y = 10;
    zappy->params->nb_team = 2;
    zappy->params->nb_client = 3;
    zappy->params->freq = 100;
    zappy->params->teams = malloc(sizeof(char *) * 3);
    if (!zappy->params->teams) {
        free(zappy->params);
        free(zappy);
        return NULL;
    }
    zappy->params->teams[0] = strdup("Team1");
    zappy->params->teams[1] = strdup("Team2");
    zappy->params->teams[2] = NULL;
    return zappy;
}

static player_t *new_player(int id, int x, int y, player_t *next)
{
    player_t *player = malloc(sizeof(player_t));
    if (!player)
        return NULL;

    player->id = id;
    player->posX = x;
    player->posY = y;
    player->direction = 2;
    player->level = 1;
    player->inventory = malloc(sizeof(inventory_t));
    if (!player->inventory) {
        free(player);
        return NULL;
    }
    player->inventory->nbFood = 10;
    player->inventory->nbLinemate = 5;
    player->inventory->nbDeraumere = 3;
    player->inventory->nbSibur = 2;
    player->inventory->nbMendiane = 1;
    player->inventory->nbPhiras = 4;
    player->inventory->nbThystame = 0;
    player->next = next;
    return player;
}

static team_t *new_team(const char *name, int nbPlayers, team_t *next)
{
    team_t *team = malloc(sizeof(team_t));
    static int id = 1;

    if (!team)
        return NULL;
    team->name = strdup(name);
    team->nbPlayers = nbPlayers;
    team->nbPlayerAlive = nbPlayers;
    team->players = NULL;
    for (int i = 0; i < nbPlayers; i++) {
        team->players = new_player(id, 0, 0, team->players);
        id++;
    }
    team->next = next;
    return team;
}

static game_t *new_game(zappy_t *zappy)
{
    game_t *game = malloc(sizeof(game_t));
    if (!game)
        return NULL;
    game->map = malloc(sizeof(map_t));
    if (!game->map) {
        free(game);
        return NULL;
    }
    game->map->width = 10;
    game->map->height = 10;
    game->map->tiles = malloc(game->map->height * sizeof(inventory_t *));
    for (int i = 0; i < game->map->height; i++) {
        game->map->tiles[i] = calloc(game->map->width, sizeof(inventory_t));
        for (int j = 0; j < game->map->width; j++) {
            game->map->tiles[i][j].nbFood = 2;
            game->map->tiles[i][j].nbLinemate = 3;
            game->map->tiles[i][j].nbDeraumere = 1;
            game->map->tiles[i][j].nbSibur = 1;
            game->map->tiles[i][j].nbMendiane = 0;
            game->map->tiles[i][j].nbPhiras = 0;
            game->map->tiles[i][j].nbThystame = 5;
        }
    }
    game->teams = NULL;
    for (int i = 0; i < zappy->params->nb_team; i++) {
        game->teams = new_team(zappy->params->teams[i], zappy->params->nb_client, game->teams);
    }
    return game;
}

static graph_net_t *new_gui()
{
    graph_net_t *gui = malloc(sizeof(graph_net_t));

    if (!gui)
        return NULL;
    gui->network = malloc(sizeof(network_t));
    gui->network->fd = open("gui_socket", O_RDWR | O_CREAT, 0666);
    gui->mapSent = true;
    gui->next = NULL;
    return gui;
}

static zappy_t *default_zappy(void)
{
    zappy_t *zappy = new_zappy();

    if (!zappy)
        return NULL;
    zappy->game = new_game(zappy);
    if (!zappy->game) {
        free(zappy->params->teams[0]);
        free(zappy->params->teams[1]);
        free(zappy->params->teams);
        free(zappy->params);
        free(zappy);
        return NULL;
    }
    zappy->network = NULL;
    zappy->graph = new_gui();
    if (!zappy->graph) {
        free(zappy->game->map);
        free(zappy->game);
        free(zappy->params->teams[0]);
        free(zappy->params->teams[1]);
        free(zappy->params->teams);
        free(zappy->params);
        free(zappy);
        return NULL;
    }
    return zappy;
}

// Test for sst command

Test(sst, valid_command, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    char message[] = "sst 80\n";
    int result;

    cr_assert_not_null(zappy);
    result = sst(zappy, zappy->graph, message);
    (void)result; // Ignore return value for this test
}

Test(sst, invalid_command_only_sst, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    char message[] = "sst\n";
    int result;

    cr_assert_not_null(zappy);
    result = sst(zappy, zappy->graph, message);
    cr_assert_eq(result, -1);
    remove("gui_socket");
}

Test(sst, invalid_command_pattern, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    char message[] = "sst ezrg\n";
    int result;

    cr_assert_not_null(zappy);
    result = sst(zappy, zappy->graph, message);
    cr_assert_eq(result, -1);
    remove("gui_socket");
}

Test(sst, invalid_file_descriptor, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    char message[] = "sst 20\n";
    int result;

    cr_assert_not_null(zappy);
    zappy->graph->network->fd = -1;
    result = sst(zappy, zappy->graph, message);
    cr_assert_eq(result, -1);
    remove("gui_socket");
}
