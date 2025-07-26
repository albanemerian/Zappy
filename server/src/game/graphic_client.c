/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** graphic_client
*/

#include "zappy.h"
#include "network.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const graphic_pf_t GRAPHIC_COMMAND[] = {
    {"msz", &msz},
    {"bct", &bct},
    {"mct", &mct},
    {"tna", &tna},
    {"ppo", &ppo},
    {"plv", &plv},
    {"plu", &plu},
    {"pld", &pld},
    {"pin", &pin},
    {"sgt", &sgt},
    {"sst", &sst},
    {"kil", &kil},
    {"tar", &tar},
    {"tsr", &tsr},
    {"pia", &pia},
    {"pis", &pis},
    {NULL, NULL}
};

static void *free_net_reading(graph_net_t *new_node)
{
    free(new_node->network);
    free(new_node);
    return NULL;
}

static void *free_net_writting(graph_net_t *new_node)
{
    free(new_node->network->readingBuffer);
    free(new_node->network);
    free(new_node);
    return NULL;
}

graph_net_t *add_graph_node(graph_net_t **head, int fd)
{
    graph_net_t *new_node = malloc(sizeof(graph_net_t));

    if (!new_node)
        return NULL;
    new_node->network = malloc(sizeof(network_t));
    if (!new_node->network) {
        free(new_node);
        return NULL;
    }
    new_node->network->readingBuffer = create_buffer('\n');
    if (!new_node->network->readingBuffer)
        return free_net_reading(new_node);
    new_node->network->writingBuffer = create_buffer('\n');
    if (!new_node->network->writingBuffer)
        return free_net_writting(new_node);
    new_node->network->fd = fd;
    new_node->mapSent = false;
    new_node->next = *head;
    *head = new_node;
    return new_node;
}

static graph_net_t *find_node(graph_net_t *current, graph_net_t *prev,
    graph_net_t **head, int fd)
{
    if (current->network->fd == fd) {
        if (prev)
            prev->next = current->next;
        else
            *head = current->next;
        current->next = NULL;
        return current;
    }
    return current;
}

graph_net_t *remove_graph_node(graph_net_t **head, int fd)
{
    graph_net_t *current = *head;
    graph_net_t *prev = NULL;

    while (current) {
        current = find_node(current, prev, head, fd);
        prev = current;
        current = current->next;
    }
    return NULL;
}

int poll_graphic_commands(zappy_t *zappy, graph_net_t *current,
    char *buffer)
{
    for (int i = 0; GRAPHIC_COMMAND[i].command != NULL; i++) {
        if (strncmp(GRAPHIC_COMMAND[i].command, buffer,
            strlen(GRAPHIC_COMMAND[i].command)) == 0)
            return GRAPHIC_COMMAND[i].handler(zappy, current, buffer);
    }
    return -1;
}
