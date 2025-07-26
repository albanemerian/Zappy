/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** incantation
*/


#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This table defines the prerequisite of the incantation*/
static const incantation_t INCANTATION[] = {
    {2, 1, {0, 1, 0, 0, 0, 0, 0}},
    {3, 2, {0, 1, 1, 1, 0, 0, 0}},
    {4, 2, {0, 2, 0, 1, 0, 2, 0}},
    {5, 4, {0, 1, 1, 2, 0, 1, 0}},
    {6, 4, {0, 1, 2, 1, 3, 0, 0}},
    {7, 6, {0, 1, 2, 3, 0, 1, 0}},
    {8, 6, {0, 2, 2, 2, 2, 2, 1}}
};

static int get_incantation_index(int level_to)
{
    for (int i = 0; i < 7; i++) {
        if (INCANTATION[i].levelt_to_reach == level_to)
            return i;
    }
    return -1;
}

static bool check_resources_on_tile(inventory_t *tile,
    const inventory_t *required)
{
    if (tile->nbLinemate < required->nbLinemate ||
        tile->nbDeraumere < required->nbDeraumere ||
        tile->nbSibur < required->nbSibur)
        return false;
    if (tile->nbMendiane < required->nbMendiane ||
        tile->nbPhiras < required->nbPhiras ||
        tile->nbThystame < required->nbThystame)
        return false;
    return true;
}

static int validate_incantation_requirements(player_t *player, zappy_t *zappy,
    int index)
{
    inventory_t *tile = &zappy->game->map->tiles[player->posX][player->posY];
    int nb_players = get_nb_player_on_tile(player->posX, player->posY, zappy,
        player->level);

    if (!check_resources_on_tile(tile, &INCANTATION[index].required_inventory))
        return -1;
    if (nb_players < INCANTATION[index].nb_players)
        return -1;
    return nb_players;
}

/* This function checks the current player on the team and there level */
int check_player_on_tile(player_t *player, zappy_t *zappy)
{
    int level_to = player->level + 1;
    int index = get_incantation_index(level_to);

    if (index == -1)
        return -1;
    return validate_incantation_requirements(player, zappy, index);
}

/* This function removes the crystalls from the tile */
static void remove_required_crystals(inventory_t *tile,
    const inventory_t *required)
{
    for (int i = 0; i < required->nbLinemate; i++)
        rm_linemate(tile);
    for (int i = 0; i < required->nbDeraumere; i++)
        rm_deraumere(tile);
    for (int i = 0; i < required->nbSibur; i++)
        rm_sibur(tile);
}

static void remove_additional_crystals(inventory_t *tile,
    const inventory_t *required)
{
    for (int i = 0; i < required->nbMendiane; i++)
        rm_mendiane(tile);
    for (int i = 0; i < required->nbPhiras; i++)
        rm_phiras(tile);
    for (int i = 0; i < required->nbThystame; i++)
        rm_thystame(tile);
}

void remove_crystal_from_tiles(int posX, int posY, int level,
    zappy_t *zappy)
{
    int index = get_incantation_index(level + 1);
    inventory_t *tile = &zappy->game->map->tiles[posX][posY];

    if (index == -1)
        return;
    remove_required_crystals(tile, &INCANTATION[index].required_inventory);
    remove_additional_crystals(tile, &INCANTATION[index].required_inventory);
    send_map_tile(zappy->game->map->tiles, zappy, posX, posY);
}

int validate_and_get_players(player_t *player, zappy_t *zappy,
    int **player_list)
{
    int nb_players = check_player_on_tile(player, zappy);

    if (nb_players == -1)
        return -1;
    *player_list = get_player_on_tile_id(player->posX, player->posY,
        zappy, nb_players);
    if (*player_list == NULL)
        return -1;
    return nb_players;
}

static void send_elevation_uder_way_to_all(zappy_t *zappy, int *player_list,
    int nb_players)
{
    char *message = "Elevation underway\n";
    player_t *player = NULL;

    for (int i = 0; i < nb_players; i++) {
        player = get_player_by_id(zappy->game, player_list[i]);
        if (player && player->network) {
            write_in_buffer(player->network->writingBuffer, message);
            write_message(player->network);
        }
    }
}

/* This functuion start the incantation returns -2 = "Elevation underway" */
int handle_incantation(player_t *player, char *command, zappy_t *zappy)
{
    int *player_list = NULL;
    int nb_players = validate_and_get_players(player, zappy, &player_list);

    if (nb_players == -1) {
        send_end_incantation(zappy, player, "ko");
        return -1;
    }
    (void)command;
    send_start_incantation(zappy, player, player_list, nb_players);
    mark_players_incanting(player_list, nb_players, zappy);
    valid_message("Elevation underway");
    send_elevation_uder_way_to_all(zappy, player_list, nb_players);
    return -2;
}
