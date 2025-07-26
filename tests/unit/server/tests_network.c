/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** tests_network
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include "network.h"

static void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// Test data and helper functions
static int test_server_fd = -1;
static int test_client_fd = -1;

static void setup_sockets(void)
{
    int fds[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0) {
        test_server_fd = fds[0];
        test_client_fd = fds[1];
    }
}

static void cleanup_sockets(void)
{
    if (test_server_fd != -1) {
        close(test_server_fd);
        test_server_fd = -1;
    }
    if (test_client_fd != -1) {
        close(test_client_fd);
        test_client_fd = -1;
    }
}

// Tests for create_server.c functions
Test(set_socket, successful_socket_creation, .init = redirect_all_std)
{
    int sockfd = set_socket();
    
    cr_assert_neq(sockfd, -1);
    cr_assert_geq(sockfd, 0);
    
    if (sockfd != -1) {
        close(sockfd);
    }
}

Test(bind_socket, successful_bind, .init = redirect_all_std)
{
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = sockfd;
    server->port = 8080; // Example port
    server->backlog = 5;
    server->pollserver.fd = sockfd;
    int result = bind_socket(server); // Port 0 for automatic assignment
    
    (void)result;
    close(sockfd);
}

Test(bind_socket, invalid_socket_fd, .init = redirect_all_std)
{
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = -1;
    server->port = 8080; // Example port
    server->backlog = 5;
    server->pollserver.fd = -1;
    int result = bind_socket(server);
    
    cr_assert_eq(result, -1);
}

Test(bind_socket, invalid_port, .init = redirect_all_std)
{
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = sockfd;
    server->port = 80; // Example port
    server->backlog = 5;
    server->pollserver.fd = sockfd;
    bind_socket(server);
}

Test(listen_socket, successful_listen, .init = redirect_all_std)
{
    int sockfd = set_socket();
    cr_expect_neq(sockfd, -1);

    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = sockfd;
    server->port = 0; // Example port
    server->backlog = 5;
    server->pollserver.fd = sockfd;
    bind_socket(server);
    int result = listen_socket(server);
    
    cr_assert_eq(result, 0);
    
    close(sockfd);
}

Test(listen_socket, invalid_socket_fd, .init = redirect_all_std)
{
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = -1;
    server->port = 5; // Example port
    server->backlog = 5;
    server->pollserver.fd = -1;
    int result = listen_socket(server);
    
    cr_assert_eq(result, -1);
}

// Tests for accept_connection.c
Test(accept_connection, invalid_server_fd, .init = redirect_all_std)
{
    server_t *server = (server_t *)malloc(sizeof(server_t));
    server->sockfd = -1;
    server->port = 8080; // Example port
    server->backlog = 5;
    server->pollserver.fd = -1;
    int result = accept_connection(server);
    
    cr_assert_eq(result, -1);
}

// Tests for handle_input.c
Test(get_fd_message, invalid_fd, .init = redirect_all_std)
{
    char *result = get_fd_message(-1);
    
    cr_assert_eq(result, NULL);
}

Test(get_fd_message, valid_message, .init = redirect_all_std, .fini = cleanup_sockets)
{
    setup_sockets();
    
    // Write test message to client fd
    const char *test_msg = "Hello World\n";
    write(test_client_fd, test_msg, strlen(test_msg));
    
    char *result = get_fd_message(test_server_fd);
    
    cr_assert_neq(result, NULL);
    cr_assert_str_eq(result, "Hello World");
    
    free(result);
}

Test(get_fd_message, empty_message, .init = redirect_all_std, .fini = cleanup_sockets)
{
    setup_sockets();
    
    // Write just newline
    write(test_client_fd, "\n", 1);
    
    char *result = get_fd_message(test_server_fd);
    
    cr_assert_neq(result, NULL);
    cr_assert_str_eq(result, "");
    
    free(result);
}

Test(get_fd_message, multiple_messages, .init = redirect_all_std, .fini = cleanup_sockets)
{
    setup_sockets();
    
    // Write multiple messages
    const char *msg1 = "First\n";
    const char *msg2 = "Second\n";
    write(test_client_fd, msg1, strlen(msg1));
    write(test_client_fd, msg2, strlen(msg2));
    
    char *result1 = get_fd_message(test_server_fd);
    char *result2 = get_fd_message(test_server_fd);
    
    cr_assert_neq(result1, NULL);
    cr_assert_str_eq(result1, "First");
    cr_assert_neq(result2, NULL);
    cr_assert_str_eq(result2, "Second");
    
    free(result1);
    free(result2);
}

Test(get_message, invalid_fd, .init = redirect_all_std)
{
    network_t network;
    network.fd = -1;
    network.readingBuffer = create_buffer('\n');
    
    char *result = get_message(&network);
    
    cr_assert_eq(result, NULL);
    
    free(network.readingBuffer);
}


Test(get_message, existing_message_in_buffer, .init = redirect_all_std)
{
    network_t network;
    network.fd = 0;
    network.readingBuffer = create_buffer('\n');
    
    // Pre-populate buffer with a complete message
    const char *test_msg = "Existing message\n";
    for (int i = 0; test_msg[i] != '\0'; i++) {
        cb_write(network.readingBuffer, test_msg[i]);
    }
    
    char *result = get_message(&network);
    
    cr_assert_neq(result, NULL);
    cr_assert_str_eq(result, "Existing message");
    
    free(result);
    free(network.readingBuffer);
}

Test(get_message, partial_message_in_buffer, .init = redirect_all_std, .fini = cleanup_sockets)
{
    setup_sockets();
    
    network_t network;
    network.fd = test_server_fd;
    network.readingBuffer = create_buffer('\n');
    
    // Pre-populate buffer with partial message (no newline)
    const char *partial_msg = "Partial";
    for (int i = 0; partial_msg[i] != '\0'; i++) {
        cb_write(network.readingBuffer, partial_msg[i]);
    }
    
    // Write completion of message to fd
    const char *completion = " message\n";
    write(test_client_fd, completion, strlen(completion));
    
    char *result = get_message(&network);
    
    cr_assert_neq(result, NULL);
    cr_assert_str_eq(result, "Partial message");
    
    free(result);
    free(network.readingBuffer);
}

Test(get_message, buffer_wrap_around, .init = redirect_all_std)
{
    network_t network;
    network.fd = 0;
    network.readingBuffer = create_buffer('\n');
    
    // Fill buffer to near capacity to test wrap-around
    for (int i = 0; i < BUFFER_SIZE - 10; i++) {
        cb_write(network.readingBuffer, 'A');
    }
    
    // Reset buffer pointers to simulate wrap-around scenario
    network.readingBuffer->head = 5;
    network.readingBuffer->tail = BUFFER_SIZE - 5;
    
    // Add a message that wraps around
    const char *wrap_msg = "Wrap\n";
    for (int i = 0; wrap_msg[i] != '\0'; i++) {
        cb_write(network.readingBuffer, wrap_msg[i]);
    }
    
    char *result = get_message(&network);
    
    // The result will contain the wrapped message
    cr_assert_neq(result, NULL);
    
    free(result);
    free(network.readingBuffer);
}

Test(get_message, empty_buffer_closed_fd, .init = redirect_all_std, .fini = cleanup_sockets)
{
    setup_sockets();
    
    network_t network;
    network.fd = test_server_fd;
    network.readingBuffer = create_buffer('\n');
    
    // Close the writing end
    close(test_client_fd);
    test_client_fd = -1;
    
    char *result = get_message(&network);
    
    cr_assert_eq(result, NULL);
    
    free(network.readingBuffer);
}

Test(get_message, long_message, .init = redirect_all_std, .fini = cleanup_sockets)
{
    setup_sockets();
    
    network_t network;
    network.fd = test_server_fd;
    network.readingBuffer = create_buffer('\n');
    
    // Create a long message (but within buffer limits)
    char long_msg[500];
    for (int i = 0; i < 499; i++) {
        long_msg[i] = 'A' + (i % 26);
    }
    long_msg[499] = '\0';
    
    // Write long message with newline
    write(test_client_fd, long_msg, strlen(long_msg));
    write(test_client_fd, "\n", 1);
    
    char *result = get_message(&network);
    
    cr_assert_neq(result, NULL);
    cr_assert_str_eq(result, long_msg);
    
    free(result);
    free(network.readingBuffer);
}

Test(get_message, multiple_newlines, .init = redirect_all_std, .fini = cleanup_sockets)
{
    setup_sockets();
    
    network_t network;
    network.fd = test_server_fd;
    network.readingBuffer = create_buffer('\n');
    
    // Write message with multiple newlines
    const char *multi_nl = "First\nSecond\n";
    write(test_client_fd, multi_nl, strlen(multi_nl));
    
    char *result1 = get_message(&network);
    char *result2 = get_message(&network);
    
    cr_assert_neq(result1, NULL);
    cr_assert_str_eq(result1, "First");
    cr_assert_neq(result2, NULL);
    cr_assert_str_eq(result2, "Second");
    
    free(result1);
    free(result2);
    free(network.readingBuffer);
}

