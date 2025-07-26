/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** print_error
*/

#include <stdio.h>

void error_print(char const *message)
{
    printf("\033[1;31m[SERVER]\033[0m \033[0;31m%s\033[0m\n", message);
}
