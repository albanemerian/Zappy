/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server :: Errors
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

#include "zappy.h"
#include "buffer.h"

int helper(void)
{
    printf("USAGE: ./zappy_server -p port -x width -y height ");
    printf("-n name1 name2 ... -c clientsNb -f freq\n\n");
    printf("---\nPort must be between 1024 and 65535.\n");
    printf("Width and height must be positive integers between 1 and 2500.\n");
    printf("Number of clients must be between 1 and 25.\n");
    printf("Frequency must be between 1 and 1000.\n");
    printf("Number of teams must be between 1 and 8.\n---\n");
    return 84;
}

void error_message(char const *message)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[26];

    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\033[0;31m%s\033[0m\t\033[1;31m%s\033[0m\n", time_str, message);
}

int return_error(char const *message)
{
    error_message(message);
    return -1;
}

void valid_message(char const *message)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[26];

    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\033[0;32m%s\033[0m\t\033[1;29m%s\033[0m\n", time_str, message);
}

void other_message(char const *message)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[26];

    strftime(time_str, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\033[0;32m%s\033[0m\t\033[1;29m%s\033[0m\n", time_str, message);
}
