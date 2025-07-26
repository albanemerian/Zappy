/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_guiincantation
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "zappy.h"
#include "network.h"
#include "fake_malloc.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
    // Ensure malloc works normally by default
    disable_malloc_failure();
}

// Mock variables
static char *last_written_message = NULL;
static int write_message_calls = 0;
static int write_message_return = 0;

// Function declarations
int send_start_incantation(zappy_t *zappy, player_t *player, int *player_list, int nb_player);
int send_end_incantation(zappy_t *zappy, player_t *player, char *result);

// Helper functions
static zappy_t *create_test_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->params = malloc(sizeof(params_t));
    zappy->graph = malloc(sizeof(graph_net_t));
    
    zappy->params->is_debug = true;
    zappy->graph = NULL; // Initialize graph to NULL for simplicity
    // zappy->graph->next = NULL;
    
    return zappy;
}

static player_t *create_test_player(int x, int y, int level)
{
    player_t *player = malloc(sizeof(player_t));
    
    player->id = 123;
    player->posX = x;
    player->posY = y;
    player->level = level;
    player->direction = NORTH;
    player->team = strdup("team1");
    
    return player;
}

static void cleanup_test_data(zappy_t *zappy, player_t *player)
{
    if (zappy) {
        free(zappy->params);
        free(zappy->graph);
        free(zappy);
    }
    if (player) {
        free(player->team);
        free(player);
    }
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
    write_message_calls = 0;
    write_message_return = 0;
}

// Tests for send_start_incantation function
Test(gui_incantation, send_start_incantation_single_player, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(5, 10, 2);
    int player_list[] = {123};
    int nb_player = 1;
    
    write_message_return = 0;
    
    int result = send_start_incantation(zappy, player, player_list, nb_player);
    
    cr_assert_eq(result, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_start_incantation_multiple_players, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(7, 3, 3);
    int player_list[] = {123, 456, 789};
    int nb_player = 3;
    
    write_message_return = 0;
    
    int result = send_start_incantation(zappy, player, player_list, nb_player);
    
    cr_assert_eq(result, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_start_incantation_no_players, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(0, 0, 1);
    int *player_list = NULL;
    int nb_player = 0;
    
    write_message_return = 0;
    
    int result = send_start_incantation(zappy, player, player_list, nb_player);
    
    cr_assert_eq(result, -1);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_start_incantation_large_coordinates, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(999, 888, 8);
    int player_list[] = {1000, 2000, 3000, 4000, 5000};
    int nb_player = 5;
    
    write_message_return = 0;
    
    int result = send_start_incantation(zappy, player, player_list, nb_player);
    
    cr_assert_eq(result, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_start_incantation_write_message_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(5, 5, 1);
    int player_list[] = {123};
    int nb_player = 1;
    
    write_message_return = -1; // Make write_message fail
    
    int result = send_start_incantation(zappy, player, player_list, nb_player);
    
    cr_assert_eq(result, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_start_incantation_multiple_graph_nodes, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(2, 4, 2);
    
    // Add second graph node
    int player_list[] = {111, 222};
    int nb_player = 2;
    
    write_message_return = 0;
    
    int result = send_start_incantation(zappy, player, player_list, nb_player);
    
    cr_assert_eq(result, 0);

    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_start_incantation_debug_off, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    zappy->params->is_debug = false;
    player_t *player = create_test_player(1, 1, 1);
    int player_list[] = {100};
    int nb_player = 1;
    
    write_message_return = 0;
    
    int result = send_start_incantation(zappy, player, player_list, nb_player);
    
    cr_assert_eq(result, 0);
    
    cleanup_test_data(zappy, player);
}

// Tests for send_end_incantation function
Test(gui_incantation, send_end_incantation_success_result, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(5, 10, 2);
    char result[] = "1";
    
    write_message_return = 0;
    
    int ret = send_end_incantation(zappy, player, result);
    
    cr_assert_eq(ret, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_end_incantation_failure_result, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(7, 3, 3);
    char result[] = "0";
    
    write_message_return = 0;
    
    int ret = send_end_incantation(zappy, player, result);
    
    cr_assert_eq(ret, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_end_incantation_large_coordinates, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(1000, 2000, 8);
    char result[] = "1";
    
    write_message_return = 0;
    
    int ret = send_end_incantation(zappy, player, result);
    
    cr_assert_eq(ret, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_end_incantation_write_message_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(5, 5, 1);
    char result[] = "1";
    
    write_message_return = -1; // Make write_message fail
    
    int ret = send_end_incantation(zappy, player, result);
    
    cr_assert_eq(ret, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_end_incantation_multiple_graph_nodes, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(2, 4, 2);
    
    char result[] = "0";
    
    write_message_return = 0;
    
    int ret = send_end_incantation(zappy, player, result);
    
    cr_assert_eq(ret, 0);

    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_end_incantation_debug_off, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    zappy->params->is_debug = false;
    player_t *player = create_test_player(1, 1, 1);
    char result[] = "1";
    
    write_message_return = 0;
    
    int ret = send_end_incantation(zappy, player, result);
    
    cr_assert_eq(ret, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_end_incantation_zero_coordinates, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(0, 0, 1);
    char result[] = "0";
    
    write_message_return = 0;
    
    int ret = send_end_incantation(zappy, player, result);
    
    cr_assert_eq(ret, 0);
    
    cleanup_test_data(zappy, player);
}

Test(gui_incantation, send_end_incantation_edge_case_result, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(10, 20, 5);
    char result[] = "ko";
    
    write_message_return = 0;
    
    int ret = send_end_incantation(zappy, player, result);
    
    cr_assert_eq(ret, 0);
    
    cleanup_test_data(zappy, player);
}

// Malloc failure tests
Test(gui_incantation_malloc_failures, send_start_incantation_malloc_fail, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(5, 5, 1);
    int player_list[] = {123};
    int nb_player = 1;
    
    // Enable malloc failure for internal allocations
    enable_malloc_failure(0);
    
    int result = send_start_incantation(zappy, player, player_list, nb_player);
    
    // Should handle malloc failure gracefully
    cr_assert_eq(result, -1);
    
    // Disable malloc failure for cleanup
    disable_malloc_failure();
    cleanup_test_data(zappy, player);
}

Test(gui_incantation_malloc_failures, send_end_incantation_malloc_fail, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    player_t *player = create_test_player(5, 5, 1);
    char result[] = "1";
    
    // Enable malloc failure for internal allocations
    enable_malloc_failure(0);
    
    int ret = send_end_incantation(zappy, player, result);
    
    // Should handle malloc failure gracefully
    cr_assert_eq(ret, -1);
    
    // Disable malloc failure for cleanup
    disable_malloc_failure();
    cleanup_test_data(zappy, player);
}

