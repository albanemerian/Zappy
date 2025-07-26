/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CLI
*/

#include <string>
#include <iostream>
#include <sstream>

#include "CLI.hpp"
#include "../Exceptions/Exceptions.hpp"

CLI::CLI(int ac, const char *const *av) : _ac(ac), _av(av)
{
}

CLI::~CLI()
{
}

zappy::structs::Config CLI::parseArguments(int ac, const char *const *av) const
{
    if (!hasCorrectNumberOfArguments(ac))
        throw Exceptions::CLIInvalidArgumentException("Expected 5 arguments.");

    zappy::structs::Config config;
    bool portFound = false;
    bool hostFound = false;

    for (int i = 1; i < ac; i++) {
        std::string arg(av[i]);

        if (arg == "-p") {
            config.port = parsePort(av[i + 1]);
            portFound = true;
            i++;
        } else if (arg == "-h") {
            config.hostname = parseHostname(av[i + 1]);
            hostFound = true;
            i++;
        } else {
            throw Exceptions::CLIInvalidArgumentException("Unknown option: " + arg);
        }
    }

    validateConfig(portFound, hostFound);
    return config;
}

bool CLI::hasCorrectNumberOfArguments(int ac) const
{
    return ac == 5;
}

int CLI::parsePort(const char *portStr) const
{
    try {
        int port = std::stoi(portStr);

        if (port <= 0)
            throw Exceptions::CLIPortException("Port must be a positive integer");

        if (port > 65535)
            throw Exceptions::CLIPortException("Port must be less than or equal to 65535");

        return port;
    } catch (const std::invalid_argument &) {
        throw Exceptions::CLIPortException("Port must be a valid number");
    } catch (const std::out_of_range &) {
        throw Exceptions::CLIPortException("Port is out of range");
    }
}

std::string CLI::parseHostname(const char *hostnameStr) const
{
    std::string hostname(hostnameStr);
    if (hostname.empty())
        throw Exceptions::CLIHostException("Hostname cannot be empty");

    if (hostname == "localhost")
        return std::string("127.0.0.1");

    std::string segment;
    std::istringstream tokenStream(hostname);
    int segments = 0;

    while (std::getline(tokenStream, segment, '.')) {
        segments++;

        try {
            int value = std::stoi(segment);

            if (value < 0 || value > 255)
                throw Exceptions::CLIHostException(
                    "IP address segments must be between 0 and 255");
        } catch (const std::exception &) {
            throw Exceptions::CLIHostException(
                "IP address must contain valid numeric segments");
        }
    }

    if (segments != 4)
        throw Exceptions::CLIHostException("IP address must have exactly 4 segments");

    return hostname;
}

void CLI::validateConfig(bool portFound, bool hostFound) const
{
    if (!portFound)
        throw Exceptions::CLIMissingArgumentException("Missing required option: -p port");

    if (!hostFound)
        throw Exceptions::CLIMissingArgumentException("Missing required option: -h machine");
}
