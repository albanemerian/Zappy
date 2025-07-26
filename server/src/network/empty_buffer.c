/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** elpty_buffer
*/

#include "buffer.h"

#include <string.h>

void empty_buffer(buffer_t *cb)
{
    if (!cb)
        return;
    cb->head = 0;
    cb->tail = 0;
    cb->full = 0;
    memset(cb->data, 0, sizeof(cb->data));
    print_buffer_state(cb, "EMPTYING");
}
