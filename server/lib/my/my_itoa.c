/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** my_ito
*/

#include <stdlib.h>
#include "my.h"

/* This function allows me to go from int to char * */
char *my_itoa(unsigned int nb)
{
    char *numbers = "0123456789";
    char *str = malloc(sizeof(char) * (my_unsignedlen(nb) + 1));
    int i = my_unsignedlen(nb);

    if (str == NULL)
        return (NULL);
    str[i] = '\0';
    i--;
    while (i >= 0) {
        str[i] = numbers[nb % 10];
        nb /= 10;
        i--;
    }
    return (str);
}
