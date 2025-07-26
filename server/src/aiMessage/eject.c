/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** eject
*/


#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void apply_torus_world(player_t *player, zappy_t *zappy)
{
    if (player->posX < 0)
        player->posX = zappy->params->x - 1;
    if (player->posX >= zappy->params->x)
        player->posX = 0;
    if (player->posY < 0)
        player->posY = zappy->params->y - 1;
    if (player->posY >= zappy->params->y)
        player->posY = 0;
}

static int move_player(player_t *player, zappy_t *zappy, direction_t direction)
{
    int size = 8 + int_str_len((int)direction);
    char buffer[size];

    if (direction == NORTH)
        player->posY--;
    if (direction == SOUTH)
        player->posY++;
    if (direction == EAST)
        player->posX++;
    if (direction == WEST)
        player->posX--;
    apply_torus_world(player, zappy);
    snprintf(buffer, size + 1, "eject: %d\n", (int)direction);
    write_in_buffer(player->network->writingBuffer, buffer);
    if (write_message(player->network) == -1)
        return -1;
    return 0;
}

static int destroy_egg(egg_t *egg, player_t *ejectman)
{
    int size = 30 + int_str_len(egg->id) + int_str_len(ejectman->id);
    char buffer[size];

    snprintf(buffer, size + 1, "Egg %d was destroyed by player %d\n",
        egg->id, ejectman->id);
    valid_message(buffer);
    return 0;
}

static int eject_players(player_t *player, player_t *ejectman, zappy_t *zappy)
{
    if (player->id != ejectman->id && player->posX == ejectman->posX &&
        player->posY == ejectman->posY) {
        if (move_player(player, zappy, ejectman->direction) == -1)
            return -1;
        send_player_pos(zappy, player);
        return 1;
    }
    return 0;
}

static int loop_thru_players(player_t *player, player_t *ejectman,
    zappy_t *zappy)
{
    int n = 0;
    int result = 0;
    player_t *save = player;

    while (player) {
        n = eject_players(player, ejectman, zappy);
        if (n == -1)
            return -1;
        result += n;
        player = player->next;
    }
    player = save;
    return result;
}

static int loop_thru_teams(player_t *ejectman, zappy_t *zappy)
{
    int n = 0;
    int result = 0;
    team_t *save = zappy->game->teams;

    while (zappy->game->teams) {
        n = loop_thru_players(zappy->game->teams->players, ejectman, zappy);
        if (n == -1)
            return -1;
        result += n;
        zappy->game->teams = zappy->game->teams->next;
    }
    zappy->game->teams = save;
    return result;
}

static int loop_thru_eggs(player_t *ejectman, zappy_t *zappy)
{
    int result = 0;
    egg_t *save = zappy->game->map->currentEggs;

    while (save) {
        if (ejectman->posX == save->posX && ejectman->posY == save->posY) {
            destroy_egg(save, ejectman);
            send_egg_death(zappy, save);
            zappy->game->map->currentEggs = kil_egg_node(
                &zappy->game->map->currentEggs, save->id);
            result++;
        }
        save = save->next;
    }
    return result;
}

int handle_eject(player_t *player, char *command, zappy_t *zappy)
{
    int n = 0;
    int result = 0;

    if (strcmp(command, "Eject") != 0) {
        error_message("Invalid command for eject handling.");
        return -1;
    }
    n = loop_thru_teams(player, zappy);
    if (n == -1)
        return -1;
    result = loop_thru_eggs(player, zappy) + n;
    if (result == 0)
        return -1;
    return 0;
}
