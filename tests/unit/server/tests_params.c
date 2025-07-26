/*
** EPITECH PROJECT, 2025
** Unit Tests
** File description:
** Server :: Params
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include "zappy.h"
#include "fake_malloc.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

/* Server :: Params */

Test(params, check_port_valid, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_port("-p", "8080", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.port, 8080);
}

Test(params, check_port_invalid, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_port("-p", "abc", &params);

    cr_assert_eq(result, false);
}

Test(params, check_width_valid, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_width("-x", "10", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.x, 10);
}

Test(params, check_height_valid, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_height("-y", "15", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.y, 15);
}

Test(params, check_client_valid, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_client("-c", "5", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.nb_client, 5);
}

Test(params, check_freq_valid, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_freq("-f", "100", &params);

    cr_assert_eq(result, true);
    cr_assert_eq(params.freq, 100);
}

Test(params, check_args_valid, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "3", "-f", "100"};
    int argc = 14;

    params_t *params = check_args(argc, argv);

    cr_assert_not_null(params);
    cr_assert_eq(params->port, 8080);
    cr_assert_eq(params->x, 10);
    cr_assert_eq(params->y, 10);
    cr_assert_eq(params->nb_client, 3);
    cr_assert_eq(params->freq, 100);
    cr_assert_eq(params->nb_team, 2);

    free_params(params);
}

Test(params, check_args_missing_port, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-x", "10", "-y", "10", "-n", "team1", "-c", "3", "-f", "100"};
    int argc = 11;

    params_t *params = check_args(argc, argv);

    cr_assert_null(params);
}

Test(check_args, check_args_invalid, .init = redirect_all_std) {
    char *argv[] = {"./server"};
    int argc = 1;

    cr_assert_null(check_args(argc, argv), "Expected invalid arguments check to pass.");
}

Test(check_args, check_args_invalid_team_name, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "", "", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected invalid team name check to pass.");
}

Test(check_args, check_name_empty_string, .init = redirect_all_std) {
    char *argv[] = {"./server", "-p", "8080", "-x", "10", "-y", "10", "-n", "", "team2", "-c", "5", "-f", "50"};
    int argc = 14;

    cr_assert_null(check_args(argc, argv), "Expected empty team name to be handled gracefully.");
}

/* Server :: Params :: Free params */

Test(free_params, free_params_null, .init = redirect_all_std) {
    void *result = free_params(NULL);
    cr_assert_null(result, "Expected free_params with NULL to return NULL.");
}

Test(free_params, free_params_valid, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));
    params->nb_team = 2;
    params->teams = malloc(3 * sizeof(char *));
    params->teams[0] = strdup("team1");
    params->teams[1] = strdup("team2");
    params->teams[2] = NULL;

    void *result = free_params(params);
    cr_assert_null(result, "Expected free_params to return NULL after freeing.");
}

/* Additional tests for better coverage */

Test(params, check_port_invalid_negative, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_port("-p", "-1", &params);

    cr_assert_eq(result, false);
}

Test(params, check_width_invalid_zero, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_width("-x", "0", &params);

    cr_assert_eq(result, false);
}

Test(params, check_height_invalid_zero, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_height("-y", "0", &params);

    cr_assert_eq(result, false);
}

Test(params, check_client_invalid_zero, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_client("-c", "0", &params);

    cr_assert_eq(result, false);
}

Test(params, check_freq_invalid_zero, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_freq("-f", "0", &params);

    cr_assert_eq(result, false);
}

Test(params, check_args_with_debug_flag, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "3", "-f", "100", "-d"};
    int argc = 15;

    params_t *params = check_args(argc, argv);

    cr_assert_not_null(params);
    cr_assert_eq(params->is_debug, true);

    free_params(params);
}

Test(params, check_args_insufficient_argc, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080"};
    int argc = 3;

    params_t *params = check_args(argc, argv);

    cr_assert_null(params);
}

// Test(params, check_args_missing_port_value, .init = redirect_all_std) {
//     char *argv[] = {"zappy_server", "-p", "-x", "10", "-y", "10", "-n", "team1", "-c", "3", "-f", "100"};
//     int argc = 12;

//     params_t *params = check_args(argc, argv);

//     cr_assert_null(params);
// }

// Test(params, check_args_missing_width_value, .init = redirect_all_std) {
//     char *argv[] = {"zappy_server", "-p", "8080", "-x", "-y", "10", "-n", "team1", "-c", "3", "-f", "100"};
//     int argc = 12;

//     params_t *params = check_args(argc, argv);

//     cr_assert_null(params);
// }

// Test(params, check_args_missing_height_value, .init = redirect_all_std) {
//     char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "-n", "team1", "-c", "3", "-f", "100"};
//     int argc = 12;

//     params_t *params = check_args(argc, argv);

//     cr_assert_null(params);
// }

// Test(params, check_args_missing_client_value, .init = redirect_all_std) {
//     char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "-c", "-f", "100"};
//     int argc = 12;

//     params_t *params = check_args(argc, argv);

//     cr_assert_null(params);
// }

// Test(params, check_args_missing_freq_value, .init = redirect_all_std) {
//     char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "-c", "3", "-f"};
//     int argc = 12;

//     params_t *params = check_args(argc, argv);

//     cr_assert_null(params);
// }


Test(params, check_args_multiple_teams, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "team3", "team4", "-c", "3", "-f", "100"};
    int argc = 16;

    params_t *params = check_args(argc, argv);

    cr_assert_not_null(params);
    cr_assert_eq(params->nb_team, 4);
    cr_assert_str_eq(params->teams[0], "team1");
    cr_assert_str_eq(params->teams[1], "team2");
    cr_assert_str_eq(params->teams[2], "team3");
    cr_assert_str_eq(params->teams[3], "team4");

    free_params(params);
}

Test(params, check_args_invalid_port_string, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "invalid", "-x", "10", "-y", "10", "-n", "team1", "-c", "3", "-f", "100"};
    int argc = 13;

    params_t *params = check_args(argc, argv);

    cr_assert_null(params);
}

Test(params, check_args_invalid_width_string, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "invalid", "-y", "10", "-n", "team1", "-c", "3", "-f", "100"};
    int argc = 13;

    params_t *params = check_args(argc, argv);

    cr_assert_null(params);
}

Test(params, check_args_invalid_height_string, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "invalid", "-n", "team1", "-c", "3", "-f", "100"};
    int argc = 13;

    params_t *params = check_args(argc, argv);

    cr_assert_null(params);
}

Test(params, check_args_invalid_client_string, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "-c", "invalid", "-f", "100"};
    int argc = 13;

    params_t *params = check_args(argc, argv);

    cr_assert_null(params);
}

Test(params, check_args_invalid_freq_string, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "-c", "3", "-f", "invalid"};
    int argc = 13;

    params_t *params = check_args(argc, argv);

    cr_assert_null(params);
}

Test(params, check_port_null_flag, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_port(NULL, "8080", &params);

    cr_assert_eq(result, false);
}

Test(params, check_port_wrong_flag, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_port("-x", "8080", &params);

    cr_assert_eq(result, false);
}

Test(params, check_port_null_value, .init = redirect_all_std) {
    params_t params = {0};
    bool result = check_port("-p", NULL, &params);

    cr_assert_eq(result, false);
}

Test(params, check_port_null_params, .init = redirect_all_std) {
    bool result = check_port("-p", "8080", NULL);

    cr_assert_eq(result, false);
}

Test(params, check_width_null_params, .init = redirect_all_std) {
    bool result = check_width("-x", "10", NULL);

    cr_assert_eq(result, false);
}

Test(params, check_height_null_params, .init = redirect_all_std) {
    bool result = check_height("-y", "10", NULL);

    cr_assert_eq(result, false);
}

Test(params, check_client_null_params, .init = redirect_all_std) {
    bool result = check_client("-c", "5", NULL);

    cr_assert_eq(result, false);
}

Test(params, check_freq_null_params, .init = redirect_all_std) {
    bool result = check_freq("-f", "100", NULL);

    cr_assert_eq(result, false);
}

Test(params, free_params_with_empty_teams, .init = redirect_all_std) {
    params_t *params = malloc(sizeof(params_t));
    params->nb_team = 0;
    params->teams = NULL;

    void *result = free_params(params);
    cr_assert_null(result);
}

// Test(params, check_args_minimum_valid, .init = redirect_all_std, .init = redirect_all_std) {
//     char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "-c", "1", "-f", "1"};
//     int argc = 13;

//     params_t *params = check_args(argc, argv);

//     cr_assert_not_null(params);
//     cr_assert_eq(params->port, 8080);
//     cr_assert_eq(params->x, 10);
//     cr_assert_eq(params->y, 10);
//     cr_assert_eq(params->nb_client, 1);
//     cr_assert_eq(params->freq, 1);
//     cr_assert_eq(params->nb_team, 1);
//     cr_assert_eq(params->is_debug, false);

//     free_params(params);
// }

// Test malloc failure in check_args
Test(params, check_args_malloc_failure, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "3", "-f", "100"};
    int argc = 14;

    // Enable malloc failure immediately
    enable_malloc_failure(0);

    params_t *params = check_args(argc, argv);

    // Should return NULL on malloc failure
    cr_assert_null(params);
    
    // Disable malloc failure for cleanup
    disable_malloc_failure();
}

// Test malloc failure during team allocation
Test(params, check_args_malloc_failure_teams, .init = redirect_all_std) {
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "10", "-n", "team1", "team2", "-c", "3", "-f", "100"};
    int argc = 14;

    // Enable malloc failure after initial allocations succeed but team allocation fails
    enable_malloc_failure(2);

    params_t *params = check_args(argc, argv);

    // Should return NULL on malloc failure
    cr_assert_null(params);
    
    // Disable malloc failure for cleanup
    disable_malloc_failure();
}
