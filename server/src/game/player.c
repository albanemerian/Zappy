/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** player
*/

#include "zappy.h"
#include "game.h"
#include "network.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This function updates player food consumption based on time */
static void update_player_food(player_t *player, zappy_t *zappy)
{
    time_t current_time = time(NULL);
    double time_elapsed = difftime(current_time, player->last_food_check);
    float time_to_wait = 126 / (double)zappy->params->freq;

    if (time_elapsed >= time_to_wait) {
        if (player->inventory->nbFood > 0) {
            player->inventory->nbFood--;
            send_player_inventory(zappy, player);
        }
        player->last_food_check = current_time;
    }
}

void remove_player_from_alive_teamate(zappy_t *zappy, player_t *player)
{
    team_t *current_team = zappy->game->teams;
    char *team_name = player->team;

    while (current_team != NULL) {
        if (strcmp(current_team->name, team_name) == 0) {
            break;
        }
        current_team = current_team->next;
    }
    if (current_team->nbPlayerAlive > 0) {
        current_team->nbPlayerAlive--;
    }
}

/* This function handles player death */
static void handle_player_death(zappy_t *zappy, player_t *player, team_t *team)
{
    player_t *current = NULL;

    write_in_buffer(player->network->writingBuffer, "dead\n");
    write_message(player->network);
    remove_player_from_alive_teamate(zappy, player);
    send_player_death(zappy, player);
    if (team->players == player) {
        team->players = player->next;
    } else {
        current = team->players;
        while (current && current->next != player) {
            current = current->next;
        }
        if (current) {
            current->next = player->next;
        }
    }
    close_client(player->network);
    free_player(player);
}

static void check_player_health_status(zappy_t *zappy,
    team_t *current, player_t *next_player)
{
    player_t *player = current->players;

    while (player != NULL) {
        next_player = player->next;
        verify_player_id(zappy, player);
        update_player_food(player, zappy);
        if (player->inventory->nbFood <= 0)
            handle_player_death(zappy, player, current);
        player = next_player;
    }
}

static void check_winning_condition(zappy_t *zappy, team_t *current)
{
    player_t *player = current->players;
    int nb_at_eight = 0;

    while (player != NULL) {
        if (player->level >= 8) {
            nb_at_eight++;
        }
        player = player->next;
    }
    if (nb_at_eight >= 6 && zappy->game->won == false) {
        send_end_game(zappy, current->name);
        zappy->game->won = true;
    }
}

static int count_eggs_for_team(team_t *team, egg_t *eggs)
{
    int count = 0;

    for (egg_t *egg = eggs; egg != NULL; egg = egg->next) {
        if (egg && team && egg->teamName && team->name &&
            strcmp(egg->teamName, team->name) == 0) {
            count++;
        }
    }
    return count;
}

static int get_id(zappy_t *zappy)
{
    int id = 0;

    for (team_t *team = zappy->game->teams; team != NULL;
        team = team->next) {
        id += count_eggs_for_team(team, zappy->game->map->currentEggs);
    }
    return id;
}

/* This functions verify the necessity of creating an egg or not */
void verify_need_for_egg(team_t *team, zappy_t *zappy)
{
    int id = 0;
    egg_t *new = NULL;
    int pos[2] = {rand() % zappy->game->map->width - 1,
        rand() % zappy->game->map->height - 1};

    if (team->nbPlayerAlive + team->nbEggs >= team->nbPlayers) {
        return;
    }
    while (team->nbPlayerAlive + team->nbEggs < team->nbPlayers) {
        id = get_id(zappy);
        new = add_egg_node(id, pos, team->name, -1);
        push_back_egg(zappy, new);
        send_egg(zappy, new);
        team->nbEggs += 1;
    }
}

/* Loop thru the player to check health and connection updates */
void check_player_status(zappy_t *zappy)
{
    team_t *current = NULL;
    player_t *next_player = NULL;

    for (current = zappy->game->teams; current != NULL;
        current = current->next) {
        check_player_health_status(zappy, current, next_player);
        check_winning_condition(zappy, current);
        verify_need_for_egg(current, zappy);
    }
}

void remove_player_from_team(team_t *team, player_t *player, int fd,
    zappy_t *zappy)
{
    player_t *prev = NULL;

    while (player) {
        if (player->network && player->network->fd != fd) {
            prev = player;
            player = player->next;
            continue;
        }
        if (prev)
            prev->next = player->next;
        else
            team->players = player->next;
        if (team->nbPlayerAlive > 0)
            team->nbPlayerAlive--;
        send_player_death(zappy, player);
        free_player(player);
        return;
    }
}
