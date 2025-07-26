/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** broadcast
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

static const direction_orientation_t orr[] = {
    {NORTH, {1, 2, 3, 4, 5, 6, 7, 8}},
    {EAST, {3, 4, 5, 6, 7, 8, 1, 2}},
    {SOUTH, {5, 6, 7, 8, 1, 2, 3, 4}},
    {WEST, {7, 8, 1, 2, 3, 4, 5, 6}}
};

static void print_broadcast_server(player_t *source, player_t *dest,
    int orientation, char *message)
{
    int l = int_str_len(source->id) + int_str_len(dest->id) +
        int_str_len(dest->posX) + int_str_len(dest->posY) + 1 + 57;
    char *d = calloc(l + 1, sizeof(char));

    if (!d) {
        error_message("Memory allocation failed for broadcast d print.");
        return;
    }
    snprintf(d, l + 1, "Player (%d) broadcasted to (%d) at (%d, %d). Dir: %d",
        source->id, dest->id, dest->posX, dest->posY, orientation);
    valid_message(d);
    free(d);
    if (message)
        free(message);
}

static int adjust_for_orientation(int direction_number,
    direction_t orientation)
{
    int dir_val = -1;

    if (direction_number == 0)
        return 0;
    if (orientation == NORTH)
        return direction_number;
    for (int i = 0; i < 4; i++) {
        if (orr[i].dir == orientation) {
            dir_val = i;
            break;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (orr[dir_val].dir == orientation && direction_number == i + 1)
            return orr[dir_val].angle[i];
    }
    return 1;
}

static void wrap_coordinates(int *dx, int *dy, int map_width, int map_height)
{
    if (*dx > map_width / 2)
        *dx -= map_width;
    else if (*dx < - map_width / 2)
        *dx += map_width;
    if (*dy > map_height / 2)
        *dy -= map_height;
    else if (*dy < - map_height / 2)
        *dy += map_height;
}

static double get_base_angle(int dx, int dy)
{
    double angle = 0;

    angle = atan2(-dy, dx) * (180.0 / M_PI);
    if (angle < 0)
        angle += 360;
    return angle;
}

static int get_orientation(int dx, int dy)
{
    double angle = get_base_angle(dx, dy);

    if (angle >= 337.5 || angle < 22.5)
        return 3;
    if (angle >= 22.5 && angle < 67.5)
        return 4;
    if (angle >= 67.5 && angle < 112.5)
        return 5;
    if (angle >= 112.5 && angle < 157.5)
        return 6;
    if (angle >= 157.5 && angle < 202.5)
        return 7;
    if (angle >= 202.5 && angle < 247.5)
        return 8;
    if (angle >= 247.5 && angle < 292.5)
        return 1;
    if (angle >= 292.5 && angle < 337.5)
        return 2;
    return 0;
}

int broadcast_text(player_t *source, player_t *dest, char *text,
    zappy_t *zappy)
{
    int d[2] = {dest->posX - source->posX, dest->posY - source->posY};
    int dir[3] = {0};
    char *message = NULL;

    wrap_coordinates(&d[0], &d[1], zappy->params->x, zappy->params->y);
    dir[0] = get_orientation(d[0], d[1]);
    if (d[0] == 0 && d[1] == 0)
        return 0;
    dir[1] = adjust_for_orientation(dir[0], dest->direction);
    dir[2] = strlen(text) + int_str_len(dir[1]) + 11;
    message = malloc(sizeof(char) * (dir[2] + 1));
    if (!message)
        return -1;
    snprintf(message, dir[2] + 1, "message %d, %s\n", dir[1], text);
    message[dir[2]] = '\0';
    write_in_buffer(dest->network->writingBuffer, message);
    if (write_message(dest->network) == -1)
        return -1;
    print_broadcast_server(source, dest, dir[1], message);
    return 0;
}

static int send_broadcast(team_t *team, player_t *source, char *text,
    zappy_t *zappy)
{
    if (team->players != source) {
        if (broadcast_text(source, team->players, text, zappy) == -1) {
            return -1;
        }
    }
    return 0;
}

static int broadcast_to_team(team_t *team, player_t *source, char *text,
    zappy_t *zappy)
{
    player_t *save_player = team->players;

    (void)text;
    while (team->players) {
        if (source->id == team->players->id) {
            team->players = team->players->next;
            continue;
        }
        if (send_broadcast(team, source, text, zappy) == -1) {
            team->players = save_player;
            return -1;
        }
        team->players = team->players->next;
    }
    team->players = save_player;
    return 0;
}

static int loop_thru_teams(player_t *source, zappy_t *zappy, char *text)
{
    team_t *save_team = zappy->game->teams;

    for (int i = 0; i < zappy->params->nb_team; i++) {
        if (broadcast_to_team(zappy->game->teams, source, text, zappy) == -1)
            return -1;
        zappy->game->teams = zappy->game->teams->next;
    }
    zappy->game->teams = save_team;
    return 0;
}

int handle_broadcast(player_t *player, char *command, zappy_t *zappy)
{
    char *text = &command[10];

    if (loop_thru_teams(player, zappy, text) == -1)
        return -1;
    if (write_message(player->network) == -1)
        return -1;
    if (send_broadcast_to_player(zappy, player, text) == -1)
        return -1;
    return 0;
}
