/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** signal
*/


#include "zappy.h"

#include <signal.h>
#include <stdio.h>

int *get_running_state(void)
{
    static int running = 1;

    return &running;
}

void handle_sigint(int sig)
{
    (void)sig;
    *get_running_state() = 0;
    printf("\033[1;33mServer stopped.\033[0m\n");
}

void handle_sigpipe(int sig)
{
    (void)sig;
    error_message("Broken pipe detected - client disconnected");
}

void setup_signal(void)
{
    struct sigaction sa;
    struct sigaction sa_pipe;

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sa_pipe.sa_handler = handle_sigpipe;
    sigemptyset(&sa_pipe.sa_mask);
    sa_pipe.sa_flags = 0;
    sigaction(SIGPIPE, &sa_pipe, NULL);
}
