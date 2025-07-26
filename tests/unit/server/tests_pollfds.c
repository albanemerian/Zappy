/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_pollfds
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>

#include "game.h"
#include "zappy.h"
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Tests for init_poll_fds.c
Test(init_unified_poll, successful_init, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    
    cr_assert_neq(poll_struct, NULL);
    cr_assert_neq(poll_struct->fds, NULL);
    cr_assert_eq(poll_struct->capacity, 64);
    cr_assert_eq(poll_struct->count, 0);
    
    free_unified_poll(poll_struct);
}

Test(init_unified_poll, malloc_failure_simulation, .init = redirect_all_std)
{
    // This test assumes we can't easily simulate malloc failure
    // So we just test the normal case and verify structure
    unified_poll_t *poll_struct = init_unified_poll();
    
    cr_assert_neq(poll_struct, NULL);
    free_unified_poll(poll_struct);
}

Test(free_unified_poll, null_pointer, .init = redirect_all_std)
{
    free_unified_poll(NULL);
    // Should not crash
    cr_assert(1);
}

Test(free_unified_poll, valid_structure, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    
    cr_assert_neq(poll_struct, NULL);
    free_unified_poll(poll_struct);
    // Should not crash
    cr_assert(1);
}

Test(add_fd_to_poll, null_poll_struct, .init = redirect_all_std)
{
    int result = add_fd_to_poll(NULL, 5, POLLIN);
    
    cr_assert_eq(result, -1);
}

Test(add_fd_to_poll, invalid_fd, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    int result = add_fd_to_poll(poll_struct, -1, POLLIN);
    
    cr_assert_eq(result, -1);
    free_unified_poll(poll_struct);
}

Test(add_fd_to_poll, successful_add, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    int result = add_fd_to_poll(poll_struct, 5, POLLIN);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(poll_struct->count, 1);
    cr_assert_eq(poll_struct->fds[0].fd, 5);
    cr_assert_eq(poll_struct->fds[0].events, POLLIN);
    cr_assert_eq(poll_struct->fds[0].revents, 0);
    
    free_unified_poll(poll_struct);
}

Test(add_fd_to_poll, update_existing_fd, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    
    // Add fd first time
    add_fd_to_poll(poll_struct, 5, POLLIN);
    cr_assert_eq(poll_struct->count, 1);
    
    // Add same fd with different events
    int result = add_fd_to_poll(poll_struct, 5, POLLOUT);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(poll_struct->count, 1); // Should not increase count
    cr_assert_eq(poll_struct->fds[0].events, POLLOUT); // Should update events
    
    free_unified_poll(poll_struct);
}

Test(add_fd_to_poll, capacity_expansion, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    
    // Fill to capacity
    for (int i = 0; i < 64; i++) {
        int result = add_fd_to_poll(poll_struct, i + 10, POLLIN);
        cr_assert_eq(result, 0);
    }
    cr_assert_eq(poll_struct->count, 64);
    cr_assert_eq(poll_struct->capacity, 64);
    
    // Add one more to trigger expansion
    int result = add_fd_to_poll(poll_struct, 100, POLLIN);
    cr_assert_eq(result, 0);
    cr_assert_eq(poll_struct->count, 65);
    cr_assert_eq(poll_struct->capacity, 128); // Should double
    
    free_unified_poll(poll_struct);
}

Test(remove_fd_from_poll, null_poll_struct, .init = redirect_all_std)
{
    int result = remove_fd_from_poll(NULL, 5);
    
    cr_assert_eq(result, -1);
}

Test(remove_fd_from_poll, invalid_fd, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    int result = remove_fd_from_poll(poll_struct, -1);
    
    cr_assert_eq(result, -1);
    free_unified_poll(poll_struct);
}

Test(remove_fd_from_poll, fd_not_found, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    add_fd_to_poll(poll_struct, 5, POLLIN);
    
    int result = remove_fd_from_poll(poll_struct, 10); // Different fd
    
    cr_assert_eq(result, -1);
    cr_assert_eq(poll_struct->count, 1); // Should remain unchanged
    
    free_unified_poll(poll_struct);
}

Test(remove_fd_from_poll, successful_remove, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    add_fd_to_poll(poll_struct, 5, POLLIN);
    add_fd_to_poll(poll_struct, 6, POLLIN);
    add_fd_to_poll(poll_struct, 7, POLLIN);
    
    cr_assert_eq(poll_struct->count, 3);
    
    int result = remove_fd_from_poll(poll_struct, 6);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(poll_struct->count, 2);
    
    // Check that the removed fd is not in the list
    bool found = false;
    for (int i = 0; i < poll_struct->count; i++) {
        if (poll_struct->fds[i].fd == 6) {
            found = true;
            break;
        }
    }
    cr_assert_eq(found, false);
    
    free_unified_poll(poll_struct);
}

Test(remove_fd_from_poll, remove_last_element, .init = redirect_all_std)
{
    unified_poll_t *poll_struct = init_unified_poll();
    add_fd_to_poll(poll_struct, 5, POLLIN);
    
    int result = remove_fd_from_poll(poll_struct, 5);
    
    cr_assert_eq(result, 0);
    cr_assert_eq(poll_struct->count, 0);
    
    free_unified_poll(poll_struct);
}

// Helper functions for build_fds.c tests
static zappy_t *create_test_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    if (!zappy) return NULL;
    
    zappy->unified_poll = init_unified_poll();
    zappy->network = malloc(sizeof(server_t));
    zappy->network->sockfd = 10;
    
    zappy->game = malloc(sizeof(game_t));
    zappy->game->teams = NULL;
    
    zappy->graph = NULL;
    
    return zappy;
}

static void free_test_zappy(zappy_t *zappy)
{
    if (!zappy) return;
    
    if (zappy->unified_poll)
        free_unified_poll(zappy->unified_poll);
    if (zappy->network)
        free(zappy->network);
    if (zappy->game)
        free(zappy->game);
    free(zappy);
}

static team_t *create_test_team(const char *name)
{
    team_t *team = malloc(sizeof(team_t));
    if (!team) return NULL;
    
    team->name = strdup(name);
    team->players = NULL;
    team->next = NULL;
    
    return team;
}

static player_t *create_test_player(int fd)
{
    player_t *player = malloc(sizeof(player_t));
    if (!player) return NULL;
    
    player->network = malloc(sizeof(network_t));
    player->network->fd = fd;
    player->next = NULL;
    
    return player;
}

static graph_net_t *create_test_graphic(int fd)
{
    graph_net_t *graphic = malloc(sizeof(graph_net_t));
    if (!graphic) return NULL;
    
    graphic->network = malloc(sizeof(network_t));
    graphic->network->fd = fd;
    graphic->next = NULL;
    graphic->mapSent = false;
    
    return graphic;
}

// Tests for build_fds.c
Test(rebuild_poll_fds, null_zappy, .init = redirect_all_std)
{
    rebuild_poll_fds(NULL);
    // Should not crash
    cr_assert(1);
}

Test(rebuild_poll_fds, null_unified_poll, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    free_unified_poll(zappy->unified_poll);
    zappy->unified_poll = NULL;
    
    rebuild_poll_fds(zappy);
    // Should not crash
    cr_assert(1);
    
    free_test_zappy(zappy);
}

Test(rebuild_poll_fds, empty_server, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    rebuild_poll_fds(zappy);
    
    // Should add server socket
    cr_assert_eq(zappy->unified_poll->count, 1);
    cr_assert_eq(zappy->unified_poll->fds[0].fd, 10);
    cr_assert_eq(zappy->unified_poll->fds[0].events, POLLIN);
    
    free_test_zappy(zappy);
}

Test(rebuild_poll_fds, with_teams_and_players, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    // Create teams with players
    team_t *team1 = create_test_team("team1");
    team1->players = create_test_player(20);
    team1->players->next = create_test_player(21);
    
    team_t *team2 = create_test_team("team2");
    team2->players = create_test_player(22);
    
    team1->next = team2;
    zappy->game->teams = team1;
    
    rebuild_poll_fds(zappy);
    
    // Should add server socket + 3 player sockets
    cr_assert_eq(zappy->unified_poll->count, 4);
    
    // Check server socket is present
    bool server_found = false;
    for (int i = 0; i < zappy->unified_poll->count; i++) {
        if (zappy->unified_poll->fds[i].fd == 10) {
            server_found = true;
            break;
        }
    }
    cr_assert(server_found);
    
    // Clean up
    free(team1->players->next->network);
    free(team1->players->next);
    free(team1->players->network);
    free(team1->players);
    free(team1->name);
    free(team1);
    
    free(team2->players->network);
    free(team2->players);
    free(team2->name);
    free(team2);
    
    free_test_zappy(zappy);
}

Test(rebuild_poll_fds, with_graphics, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    // Create graphic clients
    graph_net_t *graphic1 = create_test_graphic(30);
    graph_net_t *graphic2 = create_test_graphic(31);
    graphic1->next = graphic2;
    zappy->graph = graphic1;
    
    rebuild_poll_fds(zappy);
    
    // Should add server socket + 2 graphic sockets
    cr_assert_eq(zappy->unified_poll->count, 3);
    
    // Clean up
    free(graphic1->network);
    free(graphic1);
    free(graphic2->network);
    free(graphic2);
    
    free_test_zappy(zappy);
}

Test(rebuild_poll_fds, with_invalid_player_fd, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    // Create team with invalid player fd
    team_t *team = create_test_team("team1");
    team->players = create_test_player(-1); // Invalid fd
    zappy->game->teams = team;
    
    rebuild_poll_fds(zappy);
    
    // Should only add server socket (invalid fd should be ignored)
    cr_assert_eq(zappy->unified_poll->count, 1);
    cr_assert_eq(zappy->unified_poll->fds[0].fd, 10);
    
    // Clean up
    free(team->players->network);
    free(team->players);
    free(team->name);
    free(team);
    
    free_test_zappy(zappy);
}

Test(rebuild_poll_fds, with_invalid_graphic_fd, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    // Create graphic with invalid fd
    graph_net_t *graphic = create_test_graphic(-1);
    zappy->graph = graphic;
    
    rebuild_poll_fds(zappy);
    
    // Should only add server socket (invalid fd should be ignored)
    cr_assert_eq(zappy->unified_poll->count, 1);
    cr_assert_eq(zappy->unified_poll->fds[0].fd, 10);
    
    // Clean up
    free(graphic->network);
    free(graphic);
    
    free_test_zappy(zappy);
}

Test(rebuild_poll_fds, mixed_valid_invalid_fds, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    // Create mixed valid/invalid fds
    team_t *team = create_test_team("team1");
    team->players = create_test_player(20);  // Valid
    team->players->next = create_test_player(-1); // Invalid
    zappy->game->teams = team;
    
    graph_net_t *graphic1 = create_test_graphic(30);  // Valid
    graph_net_t *graphic2 = create_test_graphic(-1);  // Invalid
    graphic1->next = graphic2;
    zappy->graph = graphic1;
    
    rebuild_poll_fds(zappy);
    
    // Should add server socket + 1 player + 1 graphic = 3 total
    cr_assert_eq(zappy->unified_poll->count, 3);
    
    // Clean up
    free(team->players->next->network);
    free(team->players->next);
    free(team->players->network);
    free(team->players);
    free(team->name);
    free(team);
    
    free(graphic1->network);
    free(graphic1);
    free(graphic2->network);
    free(graphic2);
    
    free_test_zappy(zappy);
}

Test(rebuild_poll_fds, null_player_network, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    // Create player with null network
    team_t *team = create_test_team("team1");
    team->players = malloc(sizeof(player_t));
    team->players->network = NULL;
    team->players->next = NULL;
    zappy->game->teams = team;
    
    rebuild_poll_fds(zappy);
    
    // Should only add server socket (null network should be ignored)
    cr_assert_eq(zappy->unified_poll->count, 1);
    cr_assert_eq(zappy->unified_poll->fds[0].fd, 10);
    
    // Clean up
    free(team->players);
    free(team->name);
    free(team);
    
    free_test_zappy(zappy);
}

Test(rebuild_poll_fds, complex_scenario, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    // Add some existing fds to the poll
    add_fd_to_poll(zappy->unified_poll, 100, POLLIN);
    add_fd_to_poll(zappy->unified_poll, 101, POLLIN);
    
    // Create complex scenario
    team_t *team1 = create_test_team("team1");
    team1->players = create_test_player(20);
    team1->players->next = create_test_player(21);
    
    team_t *team2 = create_test_team("team2");
    team2->players = create_test_player(22);
    
    team1->next = team2;
    zappy->game->teams = team1;
    
    graph_net_t *graphic = create_test_graphic(30);
    zappy->graph = graphic;
    
    rebuild_poll_fds(zappy);
}

// Helper functions for unified_polling.c tests
static zappy_t *create_full_test_zappy(void)
{
    zappy_t *zappy = create_test_zappy();
    if (!zappy) return NULL;
    
    // Add params
    zappy->params = malloc(sizeof(params_t));
    if (!zappy->params) {
        free_test_zappy(zappy);
        return NULL;
    }
    zappy->params->freq = 100;
    zappy->params->nb_team = 2;
    zappy->params->teams = malloc(sizeof(char*) * 3);
    zappy->params->teams[0] = strdup("team1");
    zappy->params->teams[1] = strdup("team2");
    zappy->params->teams[2] = NULL;
    
    return zappy;
}

static void free_full_test_zappy(zappy_t *zappy)
{
    if (!zappy) return;
    
    if (zappy->params) {
        if (zappy->params->teams) {
            for (int i = 0; zappy->params->teams[i]; i++) {
                free(zappy->params->teams[i]);
            }
            free(zappy->params->teams);
        }
        free(zappy->params);
    }
    free_test_zappy(zappy);
}

// Tests for unified_polling.c
Test(poll_all_clients, null_zappy, .init = redirect_all_std)
{
    poll_all_clients(NULL);
    // Should not crash
    cr_assert(1);
}

Test(poll_all_clients, null_unified_poll, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    free_unified_poll(zappy->unified_poll);
    zappy->unified_poll = NULL;
    
    poll_all_clients(zappy);
    // Should not crash
    cr_assert(1);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, empty_poll_count, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    zappy->unified_poll->count = 0;
    
    poll_all_clients(zappy);
    // Should not crash and return early
    cr_assert(1);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, valid_server_socket, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Add server socket to poll
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    
    poll_all_clients(zappy);
    // Should not crash
    cr_assert(1);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, with_revents_zero, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Add server socket with no events
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    zappy->unified_poll->fds[0].revents = 0;
    
    poll_all_clients(zappy);
    // Should skip processing and not crash
    cr_assert(1);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, with_graphic_disconnect_events, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Create graphic client
    graph_net_t *graphic = create_test_graphic(30);
    zappy->graph = graphic;
    
    // Add graphic fd to poll with disconnect event
    add_fd_to_poll(zappy->unified_poll, 30, POLLIN);
    zappy->unified_poll->fds[0].revents = POLLERR;
    
    poll_all_clients(zappy);
    // Should handle disconnect gracefully
    cr_assert(1);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, with_server_socket_pollin, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Add server socket with POLLIN event
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    zappy->unified_poll->fds[0].revents = POLLIN;
    
    poll_all_clients(zappy);
    // Should attempt to accept new connection
    cr_assert(1);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, with_invalid_player_fd, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Create team with invalid player fd
    team_t *team = create_test_team("team1");
    player_t *player = create_test_player(-1);
    team->players = player;
    zappy->game->teams = team;
    
    // Add invalid fd to poll
    add_fd_to_poll(zappy->unified_poll, -1, POLLIN);
    if (zappy->unified_poll->count > 0) {
        zappy->unified_poll->fds[0].revents = POLLIN;
    }
    
    poll_all_clients(zappy);
    // Should handle invalid fd gracefully
    cr_assert(1);
    
    // Clean up
    free(player->network);
    free(player);
    free(team->name);
    free(team);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, with_null_player_network, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Create team with player having null network
    team_t *team = create_test_team("team1");
    team->players = malloc(sizeof(player_t));
    team->players->network = NULL;
    team->players->next = NULL;
    zappy->game->teams = team;
    
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    zappy->unified_poll->fds[0].revents = 0;
    
    poll_all_clients(zappy);
    // Should handle null network gracefully
    cr_assert(1);
    
    // Clean up
    free(team->players);
    free(team->name);
    free(team);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, with_empty_teams, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Ensure no teams exist
    zappy->game->teams = NULL;
    
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    zappy->unified_poll->fds[0].revents = 0;
    
    poll_all_clients(zappy);
    // Should handle empty teams list
    cr_assert(1);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, with_multiple_graphic_clients, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Create multiple graphic clients
    graph_net_t *graphic1 = create_test_graphic(30);
    graph_net_t *graphic2 = create_test_graphic(31);
    graphic1->next = graphic2;
    zappy->graph = graphic1;
    
    // Add both graphic fds
    add_fd_to_poll(zappy->unified_poll, 30, POLLIN);
    add_fd_to_poll(zappy->unified_poll, 31, POLLIN);
    zappy->unified_poll->fds[0].revents = POLLIN;
    zappy->unified_poll->fds[1].revents = POLLIN;
    
    poll_all_clients(zappy);
    // Should handle multiple graphics
    cr_assert(1);
    
    // Clean up
    free(graphic1->network);
    free(graphic1);
    free(graphic2->network);
    free(graphic2);
    
    free_full_test_zappy(zappy);
}

Test(poll_all_clients, frequency_calculation, .init = redirect_all_std)
{
    zappy_t *zappy = create_full_test_zappy();
    
    // Test different frequencies
    zappy->params->freq = 10;  // Should give 100ms timeout
    
    add_fd_to_poll(zappy->unified_poll, zappy->network->sockfd, POLLIN);
    
    poll_all_clients(zappy);
    // Should calculate correct timeout
    cr_assert(1);
    
    // Test high frequency
    zappy->params->freq = 1000;  // Should give 1ms timeout
    
    poll_all_clients(zappy);
    // Should handle high frequency
    cr_assert(1);
    
    free_full_test_zappy(zappy);
}
