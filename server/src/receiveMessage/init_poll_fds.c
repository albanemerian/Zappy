/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** init_poll_fds
*/

#include <stdlib.h>

#include "zappy.h"
#include "network.h"

unified_poll_t *init_unified_poll(void)
{
    unified_poll_t *poll_struct = malloc(sizeof(unified_poll_t));

    if (!poll_struct)
        return NULL;
    poll_struct->capacity = 64;
    poll_struct->count = 0;
    poll_struct->fds = malloc(sizeof(struct pollfd) * poll_struct->capacity);
    if (!poll_struct->fds) {
        free(poll_struct);
        return NULL;
    }
    return poll_struct;
}

void free_unified_poll(unified_poll_t *poll_struct)
{
    if (!poll_struct)
        return;
    if (poll_struct->fds)
        free(poll_struct->fds);
    free(poll_struct);
}

static void init_current_fd(unified_poll_t *poll_struct, int fd, short events)
{
    poll_struct->fds[poll_struct->count].fd = fd;
    poll_struct->fds[poll_struct->count].events = events;
    poll_struct->fds[poll_struct->count].revents = 0;
    poll_struct->count++;
}

int add_fd_to_poll(unified_poll_t *poll_struct, int fd, short events)
{
    struct pollfd *new_fds = NULL;

    if (!poll_struct || fd < 0)
        return -1;
    if (poll_struct->count >= poll_struct->capacity) {
        poll_struct->capacity *= 2;
        new_fds = realloc(poll_struct->fds,
            sizeof(struct pollfd) * poll_struct->capacity);
        if (!new_fds)
            return -1;
        poll_struct->fds = new_fds;
    }
    for (int i = 0; i < poll_struct->count; i++) {
        if (poll_struct->fds[i].fd == fd) {
            poll_struct->fds[i].events = events;
            return 0;
        }
    }
    init_current_fd(poll_struct, fd, events);
    return 0;
}

int remove_fd_from_poll(unified_poll_t *poll_struct, int fd)
{
    if (!poll_struct || fd < 0)
        return -1;
    for (int i = 0; i < poll_struct->count; i++) {
        if (poll_struct->fds[i].fd == fd) {
            poll_struct->fds[i] = poll_struct->fds[poll_struct->count - 1];
            poll_struct->count--;
            return 0;
        }
    }
    return -1;
}
