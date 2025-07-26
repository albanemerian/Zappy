/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** send_message
*/

#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>

#include "network.h"

/* This function set back the buffer to zero if read intierlly */
static void reset_buffer_state(buffer_t *writingBuffer, int write_value,
    int buffer_len)
{
    if (write_value == buffer_len) {
        writingBuffer->head = 0;
        writingBuffer->tail = 0;
        writingBuffer->full = 0;
        memset(writingBuffer->data, 0, BUFFER_SIZE);
    } else
        error_print("Partial write sent to client.");
}

int write_message(network_t *network)
{
    int write_value = -1;
    int buffer_len = 0;

    if (!network || !network->writingBuffer)
        return -1;
    buffer_len = strlen(network->writingBuffer->data);
    if (buffer_len == 0)
        return 0;
    print_buffer_state(network->readingBuffer, "READING");
    write_value = write(network->fd, network->writingBuffer->data, buffer_len);
    if (write_value == -1) {
        error_print("Failed to write network->writingBuffer->data to client.");
        close(network->fd);
        return -1;
    }
    reset_buffer_state(network->writingBuffer, write_value, buffer_len);
    return 0;
}

int write_fd(int fd, const char *message)
{
    int write_value = -1;

    if (!message || fd < 0) {
        error_print("Invalid parameters for write_fd.");
        return -1;
    }
    write_value = write(fd, message, strlen(message));
    if (write_value == -1) {
        error_print("Failed to write message to file descriptor.");
        close(fd);
        return -1;
    }
    if (write_value != strlen(message)) {
        error_print("Partial write sent to file descriptor.");
    }
    return 0;
}
