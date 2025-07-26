/*
** EPITECH PROJECT, 2025
** Server
** File description:
** Server :: Parsing
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "zappy.h"

const command_pf_t CHECKERS[] = {
    {"-p", &check_port},
    {"-x", &check_width},
    {"-y", &check_height},
    {"-c", &check_client},
    {"-f", &check_freq},
    {NULL, NULL}
};

static bool check_name(char const *flag, char const *const *value,
    int nb, params_t *params)
{
    if (!flag || strcmp(flag, "-n") != 0 || !value || !params) {
        error_message("Invalid name flag.");
        return false;
    }
    params->nb_team = nb;
    params->teams = calloc(nb + 1, sizeof(char *));
    if (!params->teams) {
        error_message("Memory allocation failed for team names.");
        return false;
    }
    for (int i = 0; i < nb; i++) {
        if (value[i] == NULL || strlen(value[i]) == 0) {
            error_message("Name cannot be empty.");
            return false;
        }
        params->teams[i] = strdup(value[i]);
    }
    return true;
}

static int find_flag(int argc, char **argv, const char *flag)
{
    for (int i = 1; i < argc; i++) {
        if (argv[i] && strcmp(argv[i], flag) == 0)
            return i;
    }
    return -1;
}

static int count_names(int argc, char **argv, int start_pos)
{
    int count = 0;

    for (int i = start_pos; i < argc; i++) {
        if (argv[i][0] == '-')
            break;
        count++;
    }
    return count;
}

static bool check_frequency(const char *flag, int pos, char **argv,
    params_t *params)
{
    for (int i = 0; CHECKERS[i].flag != NULL; i++) {
        if (strcmp(CHECKERS[i].flag, flag) == 0) {
            return CHECKERS[i].checker(
                argv[pos], argv[pos + 1], params) == false;
        }
    }
    return true;
}

static bool check_simple_flag(int argc, char **argv,
    const char *flag, params_t *params)
{
    char error_msg[28];
    int pos = find_flag(argc, argv, flag);

    if (pos == -1) {
        if (strcmp(flag, "-f") != 0) {
            return true;
        }
        return false;
    }
    if (pos + 1 >= argc) {
        snprintf(error_msg, sizeof(error_msg),
            "Missing or invalid %s flag.", flag);
        error_message(error_msg);
        return true;
    }
    if (check_frequency(flag, pos, argv, params) == false)
        return false;
    return true;
}

static bool check_names_flag(int argc, char **argv, params_t *params)
{
    int pos = find_flag(argc, argv, "-n");
    int names_count;

    if (pos == -1 || pos + 1 >= argc) {
        error_message("Missing or invalid name flag.");
        return true;
    }
    names_count = count_names(argc, argv, pos + 1);
    if (names_count < 1 || names_count > 8) {
        error_message("Number of teams must be between 1 and 8.");
        return true;
    }
    return check_name(argv[pos],
        (const char **) &argv[pos + 1], names_count, params) == 0;
}

static bool check_all_params(int argc, char **argv, bool is_ok,
    params_t *params)
{
    bool port_error = check_simple_flag(argc, argv, "-p", params);
    bool width_error = check_simple_flag(argc, argv, "-x", params);
    bool height_error = check_simple_flag(argc, argv, "-y", params);
    bool names_error = check_names_flag(argc, argv, params);
    bool client_error = check_simple_flag(argc, argv, "-c", params);
    bool freq_error = check_simple_flag(argc, argv, "-f", params);

    if (port_error || width_error || height_error ||
        names_error || client_error || freq_error)
        is_ok = false;
    if (find_flag(argc, argv, "-f") == -1)
        params->freq = 100;
    return is_ok;
}

static void print_elem(params_t *params, int argc, char **argv)
{
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
            params->is_debug = true;
    }
    if (params->is_debug == false)
        return;
    printf("Port: %d\n", params->port);
    printf("Width: %d\n", params->x);
    printf("Height: %d\n", params->y);
    printf("Number of teams: %d\n", params->nb_team);
    for (int i = 0; i < params->nb_team; i++)
        printf("Team Names %d: %s\n", i + 1, params->teams[i]);
    printf("Number of clients: %d\n", params->nb_client);
    printf("Frequency: %d\n", params->freq);
    printf("Debug mode: %s\n", params->is_debug ? "Enabled" : "Disabled");
}

params_t *check_args(int argc, char **argv)
{
    bool is_ok = true;
    params_t *params = malloc(sizeof(params_t));

    if (!params)
        return NULL;
    if (argc < 11) {
        helper();
        return NULL;
    }
    params->is_debug = false;
    if (!validate_no_extra_args(argc, argv)) {
        return free_params(params);
    }
    if (!check_all_params(argc, argv, is_ok, params))
        return free_params(params);
    print_elem(params, argc, argv);
    return params;
}
