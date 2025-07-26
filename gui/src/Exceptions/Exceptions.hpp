/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Exceptions
*/

#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <exception>
#include <string>
#include "../Utils/Constants.hpp"

namespace Exceptions {

    // CLI Exceptions
    class CLIParsingException : public std::exception {
        public:
            explicit CLIParsingException(const std::string &message)
                : _message(std::string(colors::T_RED) +
                          "CLI Parsing Error: " + message +
                          colors::RESET) {}

            const char *what() const noexcept override {
                return _message.c_str();
            }

        private:
            std::string _message;
    };

    class CLIPortException : public CLIParsingException {
        public:
            explicit CLIPortException(const std::string &message)
                : CLIParsingException(std::string(colors::T_CYAN) +
                                     "Port Error: " + message +
                                     colors::RESET) {}
    };

    class CLIHostException : public CLIParsingException {
        public:
            explicit CLIHostException(const std::string &message)
                : CLIParsingException(std::string(colors::T_CYAN) +
                                     "Hostname Error: " + message +
                                     colors::RESET) {}
    };

    class CLIMissingArgumentException : public CLIParsingException {
        public:
            explicit CLIMissingArgumentException(const std::string &message)
                : CLIParsingException(std::string(colors::T_CYAN) +
                                     "Missing Argument: " + message +
                                     colors::RESET) {}
    };

    class CLIInvalidArgumentException : public CLIParsingException {
        public:
            explicit CLIInvalidArgumentException(const std::string &message)
                : CLIParsingException(std::string(colors::T_CYAN) +
                                     "Invalid Argument: " + message +
                                     colors::RESET) {}
    };

    class NetworkException : public std::exception {
        public:
            explicit NetworkException(const std::string &message)
                : _message(std::string(colors::T_RED) +
                          "Network Error: " + message +
                          colors::RESET) {}

            const char *what() const noexcept override {
                return _message.c_str();
            }

        private:
            std::string _message;
    };

    class ConnectionFailedException : public NetworkException {
        public:
            explicit ConnectionFailedException(const std::string &message)
                : NetworkException(std::string(colors::T_CYAN) +
                                  "Connection Failed: " + message +
                                  colors::RESET) {}
    };

    class SocketCreationException : public NetworkException {
        public:
            explicit SocketCreationException(const std::string &message)
                : NetworkException(std::string(colors::T_CYAN) +
                                  "Socket Creation Failed: " + message +
                                  colors::RESET) {}
    };

    class ConnectionTimeoutException : public NetworkException {
        public:
            explicit ConnectionTimeoutException(const std::string &message)
                : NetworkException(std::string(colors::T_CYAN) +
                                  "Connection Timeout: " + message +
                                  colors::RESET) {}
    };

    class SendException : public NetworkException {
        public:
            explicit SendException(const std::string &message)
                : NetworkException(std::string(colors::T_CYAN) +
                                  "Send Error: " + message +
                                  colors::RESET) {}
    };

    class ReceiveException : public NetworkException {
        public:
            explicit ReceiveException(const std::string &message)
                : NetworkException(std::string(colors::T_CYAN) +
                                  "Receive Error: " + message +
                                  colors::RESET) {}
    };

    class ModuleGraphicError : public std::exception {
        private:
            std::string _message = "";
        public:
            explicit ModuleGraphicError(const std::string &msg) : _message(msg) {};
            const char *what() const noexcept override {
                return this->_message.c_str();
            }
    };

    class ModuleAudioError : public std::exception {
        private:
            std::string _message = "";
        public:
            explicit ModuleAudioError(const std::string &msg) : _message(msg) {};
            const char *what() const noexcept override {
                return this->_message.c_str();
            }
    };
}

#endif /* !EXCEPTIONS_HPP_ */
