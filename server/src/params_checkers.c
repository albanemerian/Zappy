/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** params_checkers
*/

#include "zappy.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static int check_args_flags(char **argv, int argc, bool *consumed,
    int i)
{
    if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "-x") == 0 ||
        strcmp(argv[i], "-y") == 0 || strcmp(argv[i], "-c") == 0 ||
        strcmp(argv[i], "-f") == 0) {
        if (i + 1 >= argc) {
            free(consumed);
            return -1;
        }
        consumed[i] = true;
        consumed[i + 1] = true;
        i++;
    }
    return i;
}

static int check_name(char **argv, bool *consumed, int argc, int i)
{
    if (strcmp(argv[i], "-n") == 0) {
        consumed[i] = true;
        i++;
        if (i >= argc) {
            free(consumed);
            return -1;
        }
        while (i < argc && argv[i][0] != '-') {
            consumed[i] = true;
            i++;
        }
        i--;
    }
    return i;
}

static bool check_flag_validity(int argc, bool *consumed)
{
    for (int i = 1; i < argc; i++) {
        if (!consumed[i]) {
            error_message("Extra argument detected.");
            free(consumed);
            return false;
        }
    }
    free(consumed);
    return true;
}

bool validate_no_extra_args(int argc, char **argv)
{
    bool *consumed = calloc(argc, sizeof(bool));

    if (!consumed)
        return false;
    consumed[0] = true;
    for (int i = 1; i < argc; i++) {
        if (consumed[i])
            continue;
        i = check_args_flags(argv, argc, consumed, i);
        if (i == -1)
            return false;
        i = check_name(argv, consumed, argc, i);
        if (i == -1)
            return false;
        if (strcmp(argv[i], "-d") == 0)
            consumed[i] = true;
    }
    if (!check_flag_validity(argc, consumed))
        return false;
    return true;
}
