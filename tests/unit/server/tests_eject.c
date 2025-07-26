/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_eject
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
    zappy->params->nb_client = 1;
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
    player->network = malloc(sizeof(network_t));
    if (!player->network) {
        free(player->inventory);
        free(player);
        return NULL;
    }
    player->network->writingBuffer = malloc(sizeof(buffer_t));
    if (!player->network->writingBuffer) {
        free(player->network);
        free(player->inventory);
        free(player);
        return NULL;
    }
    player->network->writingBuffer->head = 0;
    player->network->writingBuffer->tail = 0;
    player->network->writingBuffer->full = 0;
    player->network->fd = open("player_socket", O_RDWR | O_CREAT, 0666);
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

static egg_t *init_eggs()
{
    egg_t *egg = malloc(sizeof(egg_t));
    static int egg_id = 1;

    if (!egg)
        return NULL;
    egg->id = egg_id;
    egg->posX = 0;
    egg->posY = 0;
    egg->teamName = strdup("Team1");
    egg->idLayer = 1;
    egg->isHatched = false;
    egg->next = NULL;
    egg_id++;
    return egg;
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
    game->map->currentEggs = init_eggs();
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

Test(eject, invalid_command, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    int result = handle_eject(zappy->game->teams->players, "InvalidCommand", zappy);
    cr_assert_eq(result, -1, "Should return -1 for invalid command");
}

Test(eject, no_players_to_eject, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    zappy->game->map->currentEggs = NULL; // Ensure no players are present
    zappy->game->teams->next->players = NULL; // Clear players list
    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, -1, "Should return -1 when no players to eject");
}

Test(eject, eject_player_east, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, 0, "Should return 0 when ejecting a player");

    // Check if the player's position has changed

    cr_assert_eq(zappy->game->teams->next->players->posX, 1, "Player's X position should be incremented by 1");
    cr_assert_eq(zappy->game->teams->next->players->posY, 0, "Player's Y position should remain the same");
    remove("player_socket");
    remove("gui_socket");
}

Test(eject, eject_player_north, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    zappy->game->teams->players->direction = 1; // Set initial position
    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, 0, "Should return 0 when ejecting a player");

    // Check if the player's position has changed

    cr_assert_eq(zappy->game->teams->next->players->posX, 0, "Player's X position should be incremented by 1");
    cr_assert_eq(zappy->game->teams->next->players->posY, 9, "Player's Y position should remain the same");
    remove("player_socket");
    remove("gui_socket");
}

Test(eject, eject_player_south, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    zappy->game->teams->players->direction = 3; // Set initial position
    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, 0, "Should return 0 when ejecting a player");

    // Check if the player's position has changed

    cr_assert_eq(zappy->game->teams->next->players->posX, 0, "Player's X position should be incremented by 1");
    cr_assert_eq(zappy->game->teams->next->players->posY, 1, "Player's Y position should remain the same");
    remove("player_socket");
    remove("gui_socket");
}

Test(eject, eject_player_west, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    zappy->game->teams->players->direction = 4; // Set initial position
    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, 0, "Should return 0 when ejecting a player");

    // Check if the player's position has changed

    cr_assert_eq(zappy->game->teams->next->players->posX, 9, "Player's X position should be incremented by 1");
    cr_assert_eq(zappy->game->teams->next->players->posY, 0, "Player's Y position should remain the same");
    remove("player_socket");
    remove("gui_socket");
}

Test(eject, destroy_egg, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    zappy->game->teams->players->direction = 3; // Set initial position
    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, 0, "Should return 0 when ejecting a player");

    // Check if the player's position has changed

    cr_assert_null(zappy->game->map->currentEggs, "Player's Y position should remain the same");
    remove("player_socket");
    remove("gui_socket");
}

Test(eject, eject_player_torus_east, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    zappy->game->teams->players->posX = 9; // Set initial position
    zappy->game->teams->next->players->posX = 9; // Set initial position
    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, 0, "Should return 0 when ejecting a player");

    // Check if the player's position has changed

    cr_assert_eq(zappy->game->teams->next->players->posX, 0, "Player's X position should be incremented by 1");
    cr_assert_eq(zappy->game->teams->next->players->posY, 0, "Player's Y position should remain the same");
    remove("player_socket");
    remove("gui_socket");
}

Test(eject, eject_player_torus_south, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    zappy->game->teams->players->direction = 3; // Set initial position
    zappy->game->teams->players->posY = 9; // Set initial position
    zappy->game->teams->next->players->posY = 9; // Set initial position
    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, 0, "Should return 0 when ejecting a player");

    // Check if the player's position has changed

    cr_assert_eq(zappy->game->teams->next->players->posX, 0, "Player's X position should be incremented by 1");
    cr_assert_eq(zappy->game->teams->next->players->posY, 0, "Player's Y position should remain the same");
    remove("player_socket");
    remove("gui_socket");
}

Test(eject, eject_wrong_fd, .init = redirect_all_std)
{
    zappy_t *zappy = default_zappy();
    cr_assert_not_null(zappy);

    zappy->game->teams->players->network->fd = -1; // Set initial position
    zappy->game->teams->next->players->network->fd = -1; // Set initial position
    int result = handle_eject(zappy->game->teams->players, "Eject", zappy);
    cr_assert_eq(result, -1, "Should return 0 when ejecting a player");
    remove("player_socket");
    remove("gui_socket");
}