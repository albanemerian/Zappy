/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** my_int_str_length
*/

#include <stdio.h>
#include "my.h"

/* This functions caluclate the lenght of an int as a string */
int int_str_len(int value)
{
    return snprintf(NULL, 0, "%d", value);
}
