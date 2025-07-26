/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** hanlde_input
*/

#include "buffer.h"
#include "network.h"

#include <unistd.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static int len_message(int start, int end)
{
    int message_len = 0;

    if (start == end)
        return -1;
    if (end > start)
        message_len = end - start;
    else
        message_len = (BUFFER_SIZE - start) + end;
    if (message_len <= 0)
        return -1;
    return message_len;
}

static buffer_t *get_message_from_buffer(int fd, buffer_t *cb,
    char c)
{
    while (1) {
        if (read(fd, &c, 1) <= 0)
            return NULL;
        cb_write(cb, c);
        if (c == cb->delim)
            break;
    }
    return cb;
}

static char *end_message(buffer_t *cb)
{
    int start = cb->tail;
    int end = cb->head;
    int message_len = len_message(start, end);
    char *message = NULL;
    int i = 0;

    if (message_len == -1)
        return NULL;
    message = malloc(sizeof(char) * message_len);
    if (!message)
        return NULL;
    while (start != end && i < message_len - 1) {
        message[i] = cb->data[start];
        start = (start + 1) % BUFFER_SIZE;
        i++;
    }
    message[i] = '\0';
    cb->tail = start;
    cb->full = 0;
    return message;
}

static int calculate_message_length(int start, int current)
{
    return (current >= start) ?
        (current - start + 1) :
        (BUFFER_SIZE - start + current + 1);
}

static char *extract_message_from_buffer(buffer_t *cb, int start, int current)
{
    int message_len = calculate_message_length(start, current);
    char *message = malloc(sizeof(char) * (message_len + 1));
    int i = 0;
    int pos = start;

    if (!message)
        return NULL;
    while (pos != current + 1 && i < message_len) {
        message[i] = cb->data[pos];
        pos = (pos + 1) % BUFFER_SIZE;
        i++;
    }
    if (message[i - 1] == cb->delim)
        message[i - 1] = '\0';
    return message;
}

static void update_buffer_tail(buffer_t *cb, int current)
{
    cb->tail = (current + 1) % BUFFER_SIZE;
    cb->full = 0;
}

static char *process_newline_found(buffer_t *cb, int start, int current)
{
    char *message = extract_message_from_buffer(cb, start, current);

    if (!message)
        return NULL;
    update_buffer_tail(cb, current);
    return message;
}

static char *check_existing_message(buffer_t *cb)
{
    int start = cb->tail;
    int current = start;

    while (current != cb->head || cb->full) {
        if (cb->data[current] == cb->delim)
            return process_newline_found(cb, start, current);
        current = (current + 1) % BUFFER_SIZE;
        if (current == cb->head && !cb->full)
            break;
    }
    return NULL;
}

char *get_message(network_t *network)
{
    buffer_t *cb_ptr = network->readingBuffer;
    char c = 0;
    char *existing_message = NULL;

    if (network->fd < 0)
        return NULL;
    existing_message = check_existing_message(network->readingBuffer);
    if (existing_message)
        return existing_message;
    cb_ptr = get_message_from_buffer(network->fd, cb_ptr, c);
    if (cb_ptr == NULL)
        return NULL;
    return check_existing_message(network->readingBuffer);
}

char *get_fd_message(int fd)
{
    static buffer_t cb = {.head = 0, .tail = 0, .full = 0, .delim = '\n'};
    buffer_t *cb_ptr = &cb;
    char c = 0;

    if (fd < 0)
        return NULL;
    cb.tail = cb.head;
    cb_ptr = get_message_from_buffer(fd, cb_ptr, c);
    if (cb_ptr == NULL)
        return NULL;
    return end_message(cb_ptr);
}
