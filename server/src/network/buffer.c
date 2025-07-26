/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** buffer
*/

#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This functions allows you to wrap arround in the buffer */
int advance(int idx)
{
    return (idx + 1) % BUFFER_SIZE;
}

/* Here you can add elesm in the buffer */
void cb_write(buffer_t *cb, char c)
{
    if (!cb)
        return;
    cb->data[cb->head] = c;
    cb->head = advance(cb->head);
    if (cb->full)
        cb->tail = advance(cb->tail);
    cb->full = (cb->head == cb->tail);
}

/* This function alows you to read a buuffer */
int cb_getline(buffer_t *cb, char *line, int max_len)
{
    int i = 0;
    int idx = cb->tail;
    char c = 0;

    while (idx != cb->head || cb->full) {
        c = cb->data[idx];
        line[i] = c;
        i++;
        idx = advance(idx);
        if (c == cb->delim || i == max_len - 1) {
            line[i] = '\0';
            cb->tail = idx;
            cb->full = 0;
            return i;
        }
    }
    return 0;
}

buffer_t *create_buffer(char delim)
{
    buffer_t *buffer = malloc(sizeof(buffer_t));

    if (!buffer) {
        printf("Failed to allocate memory for buffer.\n");
        return NULL;
    }
    buffer->head = 0;
    buffer->tail = 0;
    buffer->full = 0;
    buffer->delim = delim;
    memset(buffer->data, 0, sizeof(buffer->data));
    return buffer;
}

int write_in_buffer(buffer_t *cb, const char *str)
{
    int count = 0;

    if (!cb || !str)
        return 0;
    while (*str) {
        cb_write(cb, *str);
        str++;
        count++;
    }
    print_buffer_state(cb, "WRITING");
    return count;
}
