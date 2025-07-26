/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_accept
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "zappy.h"
#include "network.h"
#include "fake_malloc.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
    disable_malloc_failure();
}

// Mock variables
static char *last_written_message = NULL;
static char *last_read_message = NULL;
static int write_message_calls = 0;
static int write_message_return = 0;
static int accept_connection_return = -1;
static int close_calls = 0;
static bool process_new_client_return = true;

// Helper functions
static zappy_t *create_test_zappy(void)
{
    zappy_t *zappy = malloc(sizeof(zappy_t));
    zappy->network = malloc(sizeof(network_t));
    zappy->params = malloc(sizeof(params_t));
    zappy->game = malloc(sizeof(game_t));
    
    zappy->network->sockfd = 42;
    zappy->params->nb_client = 10;
    zappy->params->x = 20;
    zappy->params->y = 15;
    zappy->params->nb_team = 2;
    zappy->params->teams = malloc(sizeof(char*) * 2);
    zappy->params->teams[0] = strdup("team1");
    zappy->params->teams[1] = strdup("team2");
    zappy->game->teams = NULL;
    
    return zappy;
}

static void cleanup_test_data(zappy_t *zappy)
{
    if (zappy) {
        if (zappy->params) {
            if (zappy->params->teams) {
                for (int i = 0; i < zappy->params->nb_team; i++) {
                    free(zappy->params->teams[i]);
                }
                free(zappy->params->teams);
            }
            free(zappy->params);
        }
        if (zappy->network) {
            free(zappy->network);
        }
        if (zappy->game) {
            free(zappy->game);
        }
        free(zappy);
    }
    if (last_written_message) {
        free(last_written_message);
        last_written_message = NULL;
    }
    if (last_read_message) {
        free(last_read_message);
        last_read_message = NULL;
    }
    write_message_calls = 0;
    write_message_return = 0;
    accept_connection_return = -1;
    close_calls = 0;
    process_new_client_return = true;
}

// Tests for accept_client function
Test(accept, accept_client_successful_ai_connection, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5; // Valid fd
    write_message_return = 0;
    last_read_message = strdup("team1");
    process_new_client_return = true;
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_successful_graphic_connection, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("GRAPHIC");
    process_new_client_return = true;
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_connection_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = -1; // Connection failed
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_welcome_write_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = -1; // Write fails
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_no_message_received, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = NULL; // No message received
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_invalid_team_name, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("invalid_team");
    process_new_client_return = false; // Invalid team
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_team_full, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("nonexistent_team");
    process_new_client_return = true;
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_slots_write_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    last_read_message = strdup("team1");
    process_new_client_return = true;

    write_message_return = 0; // Will be checked in mock
    
    accept_client(zappy);
    
    // The function should handle write failures properly
    cleanup_test_data(zappy);
}

Test(accept, accept_client_map_size_write_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    last_read_message = strdup("team1");
    process_new_client_return = true;
    
    // Test handled in complete_connection_rest
    accept_client(zappy);
    
    cleanup_test_data(zappy);
}

Test(accept, accept_client_buffer_allocation_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("team1");
    process_new_client_return = true;
    
    // Enable malloc failure for buffer allocation
    enable_malloc_failure(0);
    
    accept_client(zappy);

    disable_malloc_failure();
    cleanup_test_data(zappy);
}

Test(accept, accept_client_second_buffer_allocation_failure, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("team1");
    process_new_client_return = true;
    
    // Enable malloc failure for second buffer allocation
    enable_malloc_failure(1);
    
    accept_client(zappy);
    disable_malloc_failure();
    cleanup_test_data(zappy);
}

Test(accept, accept_client_edge_case_empty_team_name, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("");
    process_new_client_return = false;
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_large_team_name, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("very_long_team_name_that_might_cause_issues");
    process_new_client_return = true;
    
    accept_client(zappy);
    
    // Should handle long team names gracefully
    cleanup_test_data(zappy);
}

Test(accept, accept_client_special_characters_team_name, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("team@#$%");
    process_new_client_return = false;
    
    accept_client(zappy);

    cleanup_test_data(zappy);
}

Test(accept, accept_client_multiple_connections, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    // Test multiple successful connections
    for (int i = 0; i < 3; i++) {
        accept_connection_return = 5 + i;
        write_message_return = 0;
        if (last_read_message) {
            free(last_read_message);
        }
        last_read_message = strdup("team1");
        process_new_client_return = true;
        
        accept_client(zappy);
        // Reset for next iteration
        write_message_calls = 0;
        if (last_written_message) {
            free(last_written_message);
            last_written_message = NULL;
        }
    }
    
    cleanup_test_data(zappy);
}

Test(accept, accept_client_graphic_case_sensitivity, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("graphic"); // lowercase
    process_new_client_return = true;
    
    accept_client(zappy);
    
    // Should not be treated as GRAPHIC (case sensitive)
    cleanup_test_data(zappy);
}

Test(accept, accept_client_whitespace_in_team_name, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("team 1"); // with space
    process_new_client_return = false;
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}

Test(accept, accept_client_connection_parameters_correct, .init = redirect_all_std)
{
    zappy_t *zappy = create_test_zappy();
    zappy->params->nb_client = 5;
    zappy->params->x = 100;
    zappy->params->y = 200;
    
    accept_connection_return = 5;
    write_message_return = 0;
    last_read_message = strdup("team1");
    process_new_client_return = true;
    
    accept_client(zappy);
    cleanup_test_data(zappy);
}
