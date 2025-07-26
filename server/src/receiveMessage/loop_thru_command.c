/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** loop_thru_command
*/

#include "zappy.h"
#include "network.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This is the array of function pointer that holds all the cmds */
static const command_info_t COMMANDS[] = {
    {"Forward", 7, PRIORITY_CRITICAL, &handle_forward},
    {"Left", 7, PRIORITY_CRITICAL, &handle_left},
    {"Right", 7, PRIORITY_CRITICAL, &handle_right},
    {"Look", 7, PRIORITY_CRITICAL, &handle_look},
    {"Inventory", 1, PRIORITY_CRITICAL, &handle_inventory},
    {"Broadcast", 7, PRIORITY_CRITICAL, &handle_broadcast},
    {"Connect_nbr", 1, PRIORITY_CRITICAL, &handle_connect_nbr},
    {"Fork", 42, PRIORITY_CRITICAL, &handle_fork},
    {"Eject", 7, PRIORITY_CRITICAL, &handle_eject},
    {"Take", 7, PRIORITY_CRITICAL, &handle_take},
    {"Set", 7, PRIORITY_CRITICAL, &handle_set},
    {"Incantation", 300, PRIORITY_CRITICAL, &handle_incantation},
    {NULL, 0, 0, NULL}
};


/* This function loops thru the cmds to find the correct func */
const command_info_t *find_command_info(char *command)
{
    for (int i = 0; COMMANDS[i].command != NULL; i++) {
        if (strncmp(
            COMMANDS[i].command, command, strlen(COMMANDS[i].command)) == 0) {
            return &COMMANDS[i];
        }
    }
    return NULL;
}
