/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** is_only_digit
*/

#include <stdio.h>
#include <ctype.h>

int is_only_digits(const char *str)
{
    if (*str == '\0')
        return 0;
    while (*str) {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}
