/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** init_team
*/

#include "zappy.h"
#include "game.h"

#include <stdlib.h>
#include <string.h>

int init_teams(zappy_t *server)
{
    team_t *current = server->game->teams;

    for (int i = 0; i < server->params->nb_team; i++) {
        current = malloc(sizeof(team_t));
        if (!current) {
            error_message("Failed to allocate memory for team structure.");
            return -1;
        }
        current->name = strdup(server->params->teams[i]);
        current->nbPlayers = 0;
        current->nbPlayerAlive = 0;
        current->nbEggs = 0;
        current->players = NULL;
        current->next = server->game->teams;
        server->game->teams = current;
    }
    return 0;
}
