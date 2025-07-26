/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** network
*/

#include "buffer.h"

#include <poll.h>

#ifndef NETWORK_H_
    #define NETWORK_H_


/* Write an errro message */
void error_print(char const *message);
/* Set the socket of the file descriptor */
int set_socket(void);
/* Write for basic fd without struct */
int write_fd(int fd, const char *message);
/* Get message from the fd */
char *get_fd_message(int fd);

/* Server part of the network */
typedef struct server_s {
    int sockfd;
    int port;
    int backlog;
    struct pollfd pollserver;
} server_t;


/* Bind the file decriptor to the port */
int bind_socket(server_t *server);
/* Specify the queue the fd will use */
int listen_socket(server_t *server);
/* Close the server */
void close_server(server_t *server);


/* Struct that "handles" the network element */
typedef struct network_s {
    int fd;
    buffer_t *readingBuffer;
    buffer_t *writingBuffer;
} network_t;

/* Accept new connetion */
int accept_connection(server_t *server);
/* Handle Message input */
char *get_message(network_t *network);
/* Hello */
int write_message(network_t *network);
/* Close fd client */
void close_client(network_t *network);

#endif /* !NETWORK_H_ */
