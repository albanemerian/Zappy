/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** gui_miscellaneous_send
*/

#include "zappy.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This function sends the end of game message */
int send_end_game(zappy_t *zappy, const char *teamName)
{
    int xLength = strlen(teamName) + 6;
    char *message = malloc(sizeof(char) * xLength);
    graph_net_t *graph = zappy->graph;

    if (message == NULL)
        return return_error("Failed to allocate memory for end game message.");
    snprintf(message, xLength, "seg %s\n", teamName);
    if (zappy->params->is_debug)
        printf("Sending end game message: %s", message);
    while (graph != NULL) {
        write_in_buffer(graph->network->writingBuffer, message);
        if (write_message(graph->network) == -1) {
            free(message);
            return return_error("Failed to write end game message.");
        }
        graph = graph->next;
    }
    free(message);
    return 0;
}

/* This sends a str Message from the server */
int send_str_message(zappy_t *zappy, const char *message)
{
    int xLength = strlen(message) + 6;
    char *formatted_message = malloc(sizeof(char) * xLength);
    graph_net_t *current = zappy->graph;

    if (formatted_message == NULL)
        return return_error("Failed to allocate memory for string message.");
    snprintf(formatted_message, xLength, "smg %s\n", message);
    if (zappy->params->is_debug)
        printf("Sending string message: %s", formatted_message);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, formatted_message);
        if (write_message(current->network) == -1) {
            free(formatted_message);
            return return_error("Failed to write end game message.");
        }
        current = current->next;
    }
    free(formatted_message);
    return 0;
}

/* This function sends unkown message */
int send_unknown_command(zappy_t *zappy)
{
    const char *message = "suc\n";
    graph_net_t *current = zappy->graph;

    if (zappy->params->is_debug)
        printf("Sending unknown command message: %s", message);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, message);
        if (write_message(current->network) == -1)
            return return_error("Failed to write end game message.");
        current = current->next;
    }
    return 0;
}

/* This send the command parameter */
int send_command_parameter(zappy_t *zappy)
{
    const char *message = "sbp\n";
    graph_net_t *current = zappy->graph;

    if (zappy->params->is_debug)
        printf("Sending command parameter message: %s", message);
    while (current != NULL) {
        write_in_buffer(current->network->writingBuffer, message);
        if (write_message(current->network) == -1) {
            return return_error("Failed to write end game message.");
        }
        current = current->next;
    }
    return 0;
}
