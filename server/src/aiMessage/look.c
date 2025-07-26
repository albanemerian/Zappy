/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** look
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int print_look_server(player_t *player)
{
    int len = int_str_len(player->id) + 24;
    char *debug = calloc(len + 1, sizeof(char));

    if (!debug) {
        error_message("Memory allocation failed for look debug print.");
        return -1;
    }
    snprintf(debug, len + 1, "Player (%d) looked around.", player->id);
    valid_message(debug);
    free(debug);
    return 0;
}

int wrap(int value, int max)
{
    return (value % max + max) % max;
}

static char *append_to_message(char *message, const char *text)
{
    size_t old_len = strlen(message);
    size_t text_len = strlen(text);

    message = realloc(message, old_len + text_len + 3);
    strcat(message, text);
    return message;
}

static char *loop_thru_teams(team_t *current_team, int *tiles,
    player_t *current_player, char *message)
{
    player_t *team_player = current_team->players;
    int tile_x = tiles[0];
    int tile_y = tiles[1];

    while (team_player != NULL) {
        if (team_player->posX == tile_x && team_player->posY == tile_y &&
            team_player != current_player) {
            message = append_to_message(message, " player");
        }
        team_player = team_player->next;
    }
    return message;
}

static char *add_players_on_tile(char *message, int *tiles,
    player_t *current_player, zappy_t *zappy)
{
    team_t *current_team = zappy->game->teams;

    while (current_team != NULL) {
        message = loop_thru_teams(current_team, tiles, current_player,
            message);
        current_team = current_team->next;
    }
    return message;
}

static char *add_resources_on_tile(char *message, inventory_t *tile)
{
    for (int i = 0; i < tile->nbFood; i++)
        message = append_to_message(message, " food");
    for (int i = 0; i < tile->nbLinemate; i++)
        message = append_to_message(message, " linemate");
    for (int i = 0; i < tile->nbDeraumere; i++)
        message = append_to_message(message, " deraumere");
    for (int i = 0; i < tile->nbSibur; i++)
        message = append_to_message(message, " sibur");
    for (int i = 0; i < tile->nbMendiane; i++)
        message = append_to_message(message, " mendiane");
    for (int i = 0; i < tile->nbPhiras; i++)
        message = append_to_message(message, " phiras");
    for (int i = 0; i < tile->nbThystame; i++)
        message = append_to_message(message, " thystame");
    return message;
}

static char *process_tile(char *message, player_t *player, zappy_t *zappy,
    int *tiles)
{
    inventory_t *current_tile = NULL;

    if (tiles == NULL)
        return message;
    current_tile = &zappy->game->map->tiles[tiles[0]][tiles[1]];
    message = add_players_on_tile(message, tiles, player, zappy);
    message = add_resources_on_tile(message, current_tile);
    message = append_to_message(message, ",");
    return message;
}

static char *finalize_message(char *message)
{
    if (strlen(message) > 1 && message[strlen(message) - 1] == ',') {
        message[strlen(message) - 1] = ' ';
        message = append_to_message(message, "]\n");
    } else {
        message = append_to_message(message, " ]\n");
    }
    return message;
}

char *look_up(player_t *player, zappy_t *zappy)
{
    char *message = strdup("[ player");
    int tile_x = 0;
    int tile_y = 0;
    int *tiles = malloc(2 * sizeof(int));
    int tiles_in_row = 0;

    if (tiles == NULL)
        NULL;
    for (int level = 0; level <= player->level; level++) {
        tiles_in_row = 2 * level + 1;
        for (int i = 0; i < tiles_in_row; i++) {
            tile_x = wrap(player->posX + (i - level), zappy->game->map->width);
            tile_y = wrap(player->posY - level, zappy->game->map->height);
            tiles[0] = tile_x;
            tiles[1] = tile_y;
            message = process_tile(message, player, zappy, tiles);
        }
    }
    return finalize_message(message);
}

/* This is the function that handle the mov in the array of function */
int handle_look(player_t *player, char *command, zappy_t *zappy)
{
    char *message = look_up(player, zappy);

    (void)command;
    if (message == NULL)
        return -1;
    empty_buffer(player->network->writingBuffer);
    write_in_buffer(player->network->writingBuffer, message);
    if (write_message(player->network) == -1) {
        free(message);
        return -1;
    }
    if (print_look_server(player) == -1) {
        free(message);
        return -1;
    }
    free(message);
    return 1;
}
