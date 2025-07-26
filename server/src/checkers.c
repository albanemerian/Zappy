/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Checkers
*/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "zappy.h"

bool check_port(char const *flag, char const *value, params_t *params)
{
    int port = 0;

    if (!params) {
        error_message("Params structure is NULL.");
        return false;
    }
    if (!flag || strcmp(flag, "-p") != 0 || !value) {
        error_message("Invalid port flag.");
        return false;
    }
    if (is_only_digits(value) == 0)
        return false;
    port = atoi(value);
    if (strlen(value) > 6 || port < 1024 || port > 65535) {
        error_message("Port must be between 1024 and 65535 (included).");
        return false;
    }
    params->port = port;
    return true;
}

bool check_width(char const *flag, char const *value, params_t *params)
{
    int width = 0;

    if (!params) {
        error_message("Params structure is NULL.");
        return false;
    }
    if (!flag || strcmp(flag, "-x") != 0 || !value) {
        error_message("Invalid width flag.");
        return false;
    }
    if (is_only_digits(value) == 0)
        return false;
    width = atoi(value);
    if (strlen(value) > 5 || width <= 0 || width > 1000) {
        error_message("Width must be a positive integer between 1 and 1000.");
        return false;
    }
    params->x = width;
    return true;
}

bool check_height(char const *flag, char const *value, params_t *params)
{
    int height = 0;

    if (!params) {
        error_message("Params structure is NULL.");
        return false;
    }
    if (!flag || strcmp(flag, "-y") != 0 || !value) {
        error_message("Invalid height flag.");
        return false;
    }
    if (is_only_digits(value) == 0)
        return false;
    height = atoi(value);
    if (strlen(value) > 5 || height <= 0 || height > 1000) {
        error_message("Height must be a positive integer between 1 and 1000.");
        return false;
    }
    params->y = height;
    return true;
}

bool check_client(char const *flag, char const *value, params_t *params)
{
    int clients_nb = 0;

    if (!params) {
        error_message("Params structure is NULL.");
        return false;
    }
    if (!flag || strcmp(flag, "-c") != 0 || !value) {
        error_message("Invalid client flag.");
        return false;
    }
    if (is_only_digits(value) == 0)
        return false;
    clients_nb = atoi(value);
    if (strlen(value) > 2 || clients_nb <= 0 || clients_nb > 25) {
        error_message("Number of clients must be betwwen 1 and 25.");
        return false;
    }
    params->nb_client = clients_nb;
    return true;
}

bool check_freq(char const *flag, char const *value, params_t *params)
{
    int freq = 0;

    if (!params) {
        error_message("Params structure is NULL.");
        return false;
    }
    if (!flag || strcmp(flag, "-f") != 0 || !value) {
        error_message("Invalid frequency flag.");
        return false;
    }
    if (is_only_digits(value) == 0)
        return false;
    freq = atoi(value);
    if (strlen(value) > 5 || freq <= 0 || freq > 1000) {
        error_message("Frequency must be 1 and 1000 (included).");
        return false;
    }
    params->freq = freq;
    return true;
}
