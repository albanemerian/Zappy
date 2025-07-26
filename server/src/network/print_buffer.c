/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** print_buffer
*/

#include "buffer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* This function prints the header of the buffer */
static void print_header(buffer_t *cb, const char *label)
{
    printf("=== %s Buffer State ===\n", label ? label : "BUFFER");
    printf("Head: %d\n", cb->head);
    printf("Tail: %d\n", cb->tail);
    printf("Full: %d\n", cb->full);
    printf("Buffer size: %d\n", BUFFER_SIZE);
}

/* This fucntions checks the error values possible of the buffer */
static int error_handling(buffer_t *cb)
{
    if (!cb || (cb->head == 0 && cb->tail == 0 && cb->full == 0)) {
        return -1;
    }
    if (cb->head > BUFFER_SIZE || cb->tail > BUFFER_SIZE
        || cb->full > BUFFER_SIZE) {
            cb->head = 0;
            cb->tail = 0;
            cb->full = 0;
            memset(cb->data, 0, BUFFER_SIZE);
            return -1;
    }
    return 0;
}

static int data_size_count(buffer_t *cb)
{
    int data_count = 0;

    if (cb->full) {
        data_count = BUFFER_SIZE;
    } else if (cb->head >= cb->tail) {
        data_count = cb->head - cb->tail;
    } else {
        data_count = BUFFER_SIZE - cb->tail + cb->head;
    }
    return data_count;
}

int *print_content_buffer(buffer_t *cb, int data_count, int *result)
{
    char c = 0;

    result[0] = cb->tail;
    result[1] = 0;
    while ((result[0] != cb->head || cb->full) && result[1] < 50) {
        c = cb->data[result[0]];
        if (c == '\0')
            break;
        if (c == '\n')
            printf("\\n");
        if (c >= 32 && c <= 126)
            printf("%c", c);
        result[0] = advance(result[0]);
        result[1]++;
        if (result[0] == cb->head && !cb->full)
            break;
    }
    if (result[1] == 50 && (result[0] != cb->head || cb->full))
        printf("...");
    return result;
}

/* This function prints the stat of the buffers */
void print_buffer_state(buffer_t *cb, const char *label)
{
    int data_count = 0;
    int indexes[2] = {0};

    if (!indexes || error_handling(cb) == -1)
        return;
    print_header(cb, label);
    data_count = data_size_count(cb);
    printf("Data count: %d\n", data_count);
    printf("Content preview: \"");
    print_content_buffer(cb, data_count, indexes);
    if (indexes[1] == 50 && (indexes[0] != cb->head || cb->full)) {
        printf("...");
    }
    printf("\"\n");
    printf("========================\n");
}
