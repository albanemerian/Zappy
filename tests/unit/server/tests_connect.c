/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_connect
*/


#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}


// Function declarations
int handle_connect_nbr(player_t *player, char *command, zappy_t *zappy);

// Mock variables
static char *last_written_message = NULL;
static int write_message_calls = 0;
static int write_message_return = 0;


// Helper functions
static zappy_t *create_test_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->params = malloc(sizeof(params_t));
    zappy->game = malloc(sizeof(game_t));
    
    zappy->params->nb_client = 10; // Default 10 clients per team
    zappy->game->teams = NULL;
    
    return zappy;
}

static team_t *create_test_team(const char *name, int nb_players)
{
    team_t *team = malloc(sizeof(team_t));
    team->name = strdup(name);
    team->nbPlayers = nb_players;
    team->nbPlayerAlive = nb_players;
    team->players = NULL;
    team->next = NULL;
    
    return team;
}

static player_t *create_test_player(const char *team_name)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    
    player->id = 1;
    player->posX = 5;
    player->posY = 5;
    player->level = 1;
    player->direction = NORTH;
    player->team = strdup(team_name);
    player->network->fd = 42;
    player->next = NULL;
    
    return player;
}

static void cleanup_test_data(zappy_t *zappy, player_t *player)
{
    if (zappy) {
        if (zappy->game) {
            team_t *current_team = zappy->game->teams;
            while (current_team) {
                team_t *next = current_team->next;
                free(current_team->name);
                free(current_team);
                current_team = next;
            }
            free(zappy->game);
        }
        if (zappy->params) {
            free(zappy->params);
        }
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player->network);
        free(player);
    }
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
    write_message_calls = 0;
    write_message_return = 0;
}

// Tests for handle_connect_nbr function
Test(connect, handle_connect_nbr_team_found_full_slots, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team1");
    char command[] = "connect_nbr";
    
    // Create team with 0 players (all slots available)
    team_t *team = create_test_team("team1", 0);
    zappy->game->teams = team;
    
    write_message_return = 0;
    
    handle_connect_nbr(player, command, zappy);
    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_team_found_partial_slots, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team1");
    char command[] = "connect_nbr";
    
    // Create team with 3 players (7 slots available)
    team_t *team = create_test_team("team1", 3);
    zappy->game->teams = team;
    
    write_message_return = 0;
    
    handle_connect_nbr(player, command, zappy);
    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_team_found_no_slots, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team1");
    char command[] = "connect_nbr";
    
    // Create team with max players (0 slots available)
    team_t *team = create_test_team("team1", 10);
    zappy->game->teams = team;
    
    write_message_return = 0;
    
    handle_connect_nbr(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_team_not_found, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("nonexistent_team");
    char command[] = "connect_nbr";
    
    // Create different team
    team_t *team = create_test_team("team1", 5);
    zappy->game->teams = team;
    
    handle_connect_nbr(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_multiple_teams, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team2");
    char command[] = "connect_nbr";
    
    // Create multiple teams
    team_t *team1 = create_test_team("team1", 5);
    team_t *team2 = create_test_team("team2", 2);
    team_t *team3 = create_test_team("team3", 8);
    
    team1->next = team2;
    team2->next = team3;
    zappy->game->teams = team1;
    
    write_message_return = 0;
    
    handle_connect_nbr(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_no_teams, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team1");
    char command[] = "connect_nbr";
    
    // No teams created
    
    handle_connect_nbr(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_write_message_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team1");
    char command[] = "connect_nbr";
    
    team_t *team = create_test_team("team1", 5);
    zappy->game->teams = team;
    
    write_message_return = -1; // Make write_message fail
    
    handle_connect_nbr(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_command_parameter_ignored, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team1");
    char command[] = "connect_nbr extra parameters ignored";
    
    team_t *team = create_test_team("team1", 1);
    zappy->game->teams = team;
    
    write_message_return = 0;
    
    handle_connect_nbr(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_edge_case_zero_max_clients, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team1");
    char command[] = "connect_nbr";
    
    // Set max clients to 0
    zappy->params->nb_client = 0;
    
    team_t *team = create_test_team("team1", 0);
    zappy->game->teams = team;
    
    write_message_return = 0;
    
    handle_connect_nbr(player, command, zappy);

    cleanup_test_data(zappy, player);
}

Test(connect, handle_connect_nbr_large_numbers, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player("team1");
    char command[] = "connect_nbr";
    
    // Set large numbers to test int_str_len calculation
    zappy->params->nb_client = 1000;
    
    team_t *team = create_test_team("team1", 123);
    zappy->game->teams = team;
    
    write_message_return = 0;
    
    handle_connect_nbr(player, command, zappy);

    cleanup_test_data(zappy, player);
}
