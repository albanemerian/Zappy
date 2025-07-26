/*
** EPITECH PROJECT, 2025
** Zapppy
** File description:
** Server :: Client
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "zappy.h"
#include "algo.h"

/* This function verify the the current team name is an existing one */
static bool valid_team_name(const char *team_name, zappy_t *zappy)
{
    for (int i = 0; i < zappy->params->nb_team; i++) {
        if (strcmp(team_name, zappy->params->teams[i]) == 0) {
            return true;
        }
    }
    error_message("Invalid team name provided.");
    return false;
}

static void send_player_connect_to_gui(zappy_t *zappy, graph_net_t *fd)
{
    team_t *current_team = zappy->game->teams;
    player_t *current_player = NULL;

    while (current_team != NULL) {
        current_player = current_team->players;
        while (current_player != NULL) {
            send_player_connect_to_specific_gui(fd, current_player);
            current_player = current_player->next;
        }
        current_team = current_team->next;
    }
    if (zappy->params->nb_client < 6)
        send_str_message(zappy, "P Warning: Low number of clients connected.");
    if (zappy->params->x < 10 || zappy->params->y < 10)
        send_str_message(zappy, "M Warning: Map size is small.");
}

/* This function check if the name is graphic if not it verify the team name */
bool process_new_client(const char *team_name, int fd, zappy_t *zappy)
{
    if (strcmp(team_name, "GRAPHIC") == 0) {
        if (add_graph_node(&zappy->graph, fd) == NULL) {
            error_message("Failed to add graphic client to the list.");
            return false;
        }
        send_map_size(zappy);
        send_time_message(zappy);
        send_entrie_map(zappy);
        send_team_name(zappy);
        send_entire_egg_list(zappy);
        send_player_connect_to_gui(zappy, zappy->graph);
        valid_message("New graphic client connected.");
        return true;
    }
    return valid_team_name(team_name, zappy);
}

/* This function initialize the inventory struct of the current player */
static inventory_t *init_inventory(void)
{
    inventory_t *inventory = malloc(sizeof(inventory_t));

    if (!inventory) {
        error_message("Failed to allocate memory for player inventory.");
        return NULL;
    }
    inventory->nbLinemate = 0;
    inventory->nbDeraumere = 0;
    inventory->nbSibur = 0;
    inventory->nbMendiane = 0;
    inventory->nbPhiras = 0;
    inventory->nbThystame = 0;
    inventory->nbFood = 10;
    return inventory;
}

/* This function initialize the current player structure */
static player_t *malloc_player(const char *team)
{
    player_t *player = malloc(sizeof(player_t));

    if (!player)
        return NULL;
    player->id = -1;
    player->network = malloc(sizeof(network_t));
    if (!player->network) {
        error_message("Failed to allocate memory for player network.");
        free(player);
        return NULL;
    }
    player->network->readingBuffer = create_buffer('\n');
    player->network->writingBuffer = create_buffer('\n');
    if (!player->network->readingBuffer || !player->network->writingBuffer) {
        error_message("Failed to allocate memory for player buffer.");
        return NULL;
    }
    player->team = strdup((char *)team);
    return player;
}

/* This loops thrue the eggs list to atribute a pos to the player */
static player_t *set_player_pos(player_t *player, zappy_t *zappy)
{
    egg_t *current_egg = NULL;

    if (zappy->game->map->currentEggs == NULL)
        return player;
    current_egg = zappy->game->map->currentEggs;
    while (current_egg != NULL){
        if (current_egg->teamName != NULL &&
            strcmp(current_egg->teamName, player->team) != 0) {
            current_egg = current_egg->next;
            continue;
        }
        if (current_egg->isHatched == false) {
            player->posX = current_egg->posX;
            player->posY = current_egg->posY;
            current_egg->isHatched = true;
            break;
        }
        current_egg = current_egg->next;
    }
    return player;
}

/* This function initialize the current player structure */
static player_t *init_player(int fd, zappy_t *zappy, const char *team)
{
    player_t *player = malloc_player(team);

    if (!player)
        return NULL;
    player->network->fd = fd;
    player->level = 1;
    player->direction = (direction_t)(rand() % 4 + 1);
    player->inventory = init_inventory();
    player->pending_actions = init_action_queue();
    gettimeofday(&player->last_action_time, NULL);
    player->time_action = 0;
    player->is_busy = false;
    player->current_action = NULL;
    player->remaining_cooldown = 0;
    player->food_timer = 126;
    player->last_food_check = time(NULL);
    if (!player->inventory)
        return free_player(player);
    player = set_player_pos(player, zappy);
    return player;
}

/* This function verify that the team the client wants to join as space */
static int check_team_capacity(zappy_t *server, const char *team_name,
    player_t *new_player)
{
    while (server->game->teams) {
        if (strcmp(server->game->teams->name, team_name) == 0 &&
            server->game->teams->nbPlayers < server->params->nb_client +
                server->game->teams->nbEggs) {
            new_player->next = server->game->teams->players;
            server->game->teams->players = new_player;
            server->game->teams->nbPlayers++;
            server->game->teams->nbPlayerAlive++;
            new_player->id = -1;
            return (server->params->nb_client + server->game->teams->nbEggs)
                - server->game->teams->nbPlayers;
        }
        server->game->teams = server->game->teams->next;
    }
    return -1;
}

static team_t *add_client_team_rest(zappy_t *server, team_t *save,
    const char *team_name, player_t *new_player)
{
    if (check_team_capacity(server, team_name, new_player) == -1) {
        server->game->teams = save;
        free_player(new_player);
        return NULL;
    }
    return server->game->teams;
}

team_t *add_client_to_team(const char *team_name, int fd, zappy_t *server)
{
    player_t *new_player = init_player(fd, server, team_name);
    team_t *save = server->game->teams;
    team_t *result = NULL;

    if (!new_player) {
        close(fd);
        return NULL;
    }
    if (!add_client_team_rest(server, save, team_name, new_player)) {
        return NULL;
    }
    result = server->game->teams;
    server->game->teams = save;
    return result;
}
