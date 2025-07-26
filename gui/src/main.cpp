/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** main
*/

#include <signal.h>
#include <iostream>
#include <string>
#include <memory>

#include "Utils/Constants.hpp"
#include "Client/Client.hpp"
#include "Exceptions/Exceptions.hpp"

std::unique_ptr<Client> g_client = nullptr;

void signalHandler(int signal)
{
    if (signal == SIGINT) {
        std::cout << "\n" << colors::T_YELLOW
                  << "Received Ctrl+C, shutting down gracefully..."
                  << colors::RESET << std::endl;

        if (g_client) {
            g_client->stop();
            g_client.reset();
        }

        std::cout << colors::T_GREEN << "Cleanup completed, exiting..."
                  << colors::RESET << std::endl;
        exit(zappy::constants::SUCCESS_EXIT_CODE);
    }
}

int main(int ac, char **av)
{
    signal(SIGINT, signalHandler);

    if (ac == 2 && std::string(av[1]) == "-help") {
        std::cout << colors::T_CYAN << zappy::constants::USAGE_STRING
                  << colors::RESET << std::endl;
        return zappy::constants::SUCCESS_EXIT_CODE;
    }

    try {
        g_client = std::make_unique<Client>(ac, av);
    } catch (const Exceptions::CLIParsingException &e) {
        std::cerr << colors::T_RED << e.what()
                  << colors::RESET << std::endl;
        std::cerr << colors::T_YELLOW << zappy::constants::USAGE_STRING
                  << colors::RESET << std::endl;
        return zappy::constants::FAILURE_EXIT_CODE;
    } catch (const std::exception &e) {
        std::cerr << colors::T_RED << "An unexpected error occurred: "
                  << e.what() << colors::RESET << std::endl;
        return zappy::constants::FAILURE_EXIT_CODE;
    }

    g_client.reset();
    return zappy::constants::SUCCESS_EXIT_CODE;
}
