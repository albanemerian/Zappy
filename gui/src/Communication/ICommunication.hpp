/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** ICommunication
*/

#ifndef ICOMMUNICATION_HPP_
#define ICOMMUNICATION_HPP_

#include <string>

class ICommunication {
    public:
        virtual ~ICommunication() = default;

        virtual void sendMessage(const std::string &message) = 0;
        virtual bool hasMessages() const = 0;
        virtual std::string popMessage() = 0;
        virtual bool isConnected() const = 0;
        virtual void disconnect() = 0;
};

#endif /* !ICOMMUNICATION_HPP_ */
