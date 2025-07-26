/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** connect
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static team_t *find_team_by_name(zappy_t *zappy, const char *team_name)
{
    for (team_t *team = zappy->game->teams; team != NULL; team = team->next) {
        if (strcmp(team->name, team_name) == 0)
            return team;
    }
    return NULL;
}

static int send_nbr_slots(player_t *player, int nb_available_slots)
{
    int str_len_slots = int_str_len(nb_available_slots);
    char buffer[str_len_slots + 2];

    snprintf(buffer, sizeof(buffer), "%d\n", nb_available_slots);
    buffer[str_len_slots + 1] = '\0';
    write_in_buffer(player->network->writingBuffer, buffer);
    if (write_message(player->network) == -1)
        return -1;
    return 1;
}

int handle_connect_nbr(player_t *player, char *command, zappy_t *zappy)
{
    team_t *player_team = NULL;
    int nb_available_slots = 0;

    (void)command;
    player_team = find_team_by_name(zappy, player->team);
    if (player_team == NULL) {
        printf("Error: Team '%s' not found.\n", player->team);
        return -1;
    }
    nb_available_slots = zappy->params->nb_client - player_team->nbPlayerAlive
        + player_team->nbEggs;
    return send_nbr_slots(player, nb_available_slots);
}
