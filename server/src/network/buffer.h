/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** buffer
*/

#include <stddef.h>

#ifndef BUFFER_H_
    #define BUFFER_H_

    #define BUFFER_SIZE 1024


typedef struct buffer_s {
    char data[BUFFER_SIZE];
    int head;
    int tail;
    int full;
    char delim;
} buffer_t;

/* buffer.c */
int advance(int idx);
void cb_write(buffer_t *cb, char c);
int cb_getline(buffer_t *cb, char *line, int max_len);
buffer_t *create_buffer(char delim);
int write_in_buffer(buffer_t *cb, const char *str);
void print_buffer_state(buffer_t *cb, const char *label);
void empty_buffer(buffer_t *cb);

#endif /* !BUFFER_H_ */
