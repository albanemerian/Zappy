/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_broadcast
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <math.h>
#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Mock variables
static char *last_written_message = NULL;
static int write_message_calls = 0;
static int write_message_return = 0;
static int send_broadcast_to_all_calls = 0;
static int send_broadcast_to_all_return = 0;

// Function declarations
int handle_broadcast(player_t *player, char *command, zappy_t *zappy);

// Helper functions to create test data
static zappy_t *create_test_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->params = malloc(sizeof(params_t));
    zappy->game = malloc(sizeof(game_t));
    
    zappy->params->nb_team = 2;
    zappy->params->x = 10;
    zappy->params->y = 10;
    zappy->params->is_debug = false;
    
    // Initialize teams
    team_t *team1 = malloc(sizeof(team_t));
    team1->name = strdup("team1");
    team1->players = NULL;
    team1->nbPlayers = 0;
    team1->nbPlayerAlive = 0;
    
    team_t *team2 = malloc(sizeof(team_t));
    team2->name = strdup("team2");
    team2->players = NULL;
    team2->nbPlayers = 0;
    team2->nbPlayerAlive = 0;
    team2->next = NULL;
    
    team1->next = team2;
    zappy->game->teams = team1;
    
    return zappy;
}

static player_t *create_test_player(int id, int x, int y, const char *team_name)
{
    player_t *player = malloc(sizeof(player_t));
    player->network = malloc(sizeof(network_t));
    player->inventory = malloc(sizeof(inventory_t));
    
    player->id = id;
    player->posX = x;
    player->posY = y;
    player->level = 1;
    player->direction = NORTH;
    player->team = strdup(team_name);
    player->network->fd = 42 + id;
    player->next = NULL;
    
    memset(player->inventory, 0, sizeof(inventory_t));
    
    return player;
}

static void add_player_to_team(zappy_t *zappy, player_t *player, const char *team_name)
{
    team_t *team = zappy->game->teams;
    while (team) {
        if (strcmp(team->name, team_name) == 0) {
            player->next = team->players;
            team->players = player;
            team->nbPlayers++;
            team->nbPlayerAlive++;
            break;
        }
        team = team->next;
    }
}

static void cleanup_test_data(zappy_t *zappy)
{
    if (zappy) {
        if (zappy->game) {
            team_t *team = zappy->game->teams;
            while (team) {
                team_t *next_team = team->next;
                player_t *player = team->players;
                while (player) {
                    player_t *next_player = player->next;
                    free(player->team);
                    free(player->network);
                    free(player->inventory);
                    free(player);
                    player = next_player;
                }
                free(team->name);
                free(team);
                team = next_team;
            }
            free(zappy->game);
        }
        free(zappy->params);
        free(zappy);
    }
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
    write_message_calls = 0;
    write_message_return = 0;
    send_broadcast_to_all_calls = 0;
    send_broadcast_to_all_return = 0;
}

// Test basic broadcast functionality
Test(broadcast, handle_broadcast_single_team_single_player, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 3, 3, "team1");
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast hello world";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, handle_broadcast_multiple_teams, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver1 = create_test_player(2, 3, 3, "team1");
    player_t *receiver2 = create_test_player(3, 7, 7, "team2");
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver1, "team1");
    add_player_to_team(zappy, receiver2, "team2");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast test message";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, handle_broadcast_write_message_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 3, 3, "team1");
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = -1; // Simulate write failure
    
    char command[] = "broadcast failed message";
    int result = handle_broadcast(sender, command, zappy);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy);
}


Test(broadcast, handle_broadcast_long_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 8, 2, "team1");
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast this is a very long message that should still work properly";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

// Test orientation calculations
Test(broadcast, orientation_calculation_east, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 8, 5, "team1"); // East of sender
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast east test";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, orientation_calculation_west, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 2, 5, "team1"); // West of sender
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast west test";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, orientation_calculation_north, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 5, 2, "team1"); // North of sender
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast north test";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, orientation_calculation_south, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 5, 8, "team1"); // South of sender
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast south test";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, orientation_calculation_diagonal_northeast, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 8, 2, "team1"); // Northeast of sender
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast northeast test";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, orientation_calculation_diagonal_southwest, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 2, 8, "team1"); // Southwest of sender
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast southwest test";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, same_position_players, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 5, 5, "team1"); // Same position
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast same position";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, multiple_players_same_team, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver1 = create_test_player(2, 3, 3, "team1");
    player_t *receiver2 = create_test_player(3, 7, 7, "team1");
    player_t *receiver3 = create_test_player(4, 1, 9, "team1");
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver1, "team1");
    add_player_to_team(zappy, receiver2, "team1");
    add_player_to_team(zappy, receiver3, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast multiple receivers";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, sender_not_receiving_own_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 3, 3, "team1");
    
    // Make sender first in the list to test the condition
    add_player_to_team(zappy, receiver, "team1");
    add_player_to_team(zappy, sender, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast self exclusion test";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, empty_teams, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    
    // Don't add sender to any team (empty teams scenario)
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast empty teams";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
    free(sender->team);
    free(sender->network);
    free(sender->inventory);
    free(sender);
}

Test(broadcast, special_characters_in_message, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 6, 6, "team1");
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast !@#$%^&*()_+-={}[]|\\:;\"'<>?,.";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

Test(broadcast, broadcast_memory_allocation_test, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *sender = create_test_player(1, 5, 5, "team1");
    player_t *receiver = create_test_player(2, 1000, 1000, "team1"); // Large coordinates
    
    add_player_to_team(zappy, sender, "team1");
    add_player_to_team(zappy, receiver, "team1");
    
    write_message_return = 0;
    send_broadcast_to_all_return = 0;
    
    char command[] = "broadcast memory test with very long message content";
    int result = handle_broadcast(sender, command, zappy);
    (void)result;
    cleanup_test_data(zappy);
}

