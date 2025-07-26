/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Communication
*/

#ifndef COMMUNICATION_HPP_
#define COMMUNICATION_HPP_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <netdb.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <string>
#include <vector>

#include "../Utils/Constants.hpp"
#include "../Exceptions/Exceptions.hpp"
#include "ICommunication.hpp"

class Communication : public ICommunication {
    public:
        explicit Communication(int port, const std::string &hostname,
                const std::string &delimiter = "\n");
        ~Communication();

        void sendMessage(const std::string &message) override;
        bool hasMessages() const override;
        std::string popMessage() override;
        bool isConnected() const override;
        void disconnect() override;

    private:
        void setupConnection();
        void createSocket();
        void connectToServer();
        void setupNonBlocking();

        void startCommunicationThread();
        void communicationLoop();
        bool handlePoll();
        void processWrite();
        void processRead();

        void parseReceivedData();

        int _port;
        std::string _hostname;
        std::string _delimiter;
        std::thread _thread;
        std::mutex _mutex;
        std::condition_variable _cv;
        std::atomic<bool> _running;
        std::atomic<bool> _connected;

        std::queue<std::string> _outgoingMessages;
        std::queue<std::string> _incomingMessages;

        std::string _receiveBuffer;
        std::string _sendBuffer;

        int _socket;
        struct pollfd _pollfd;
        static const int BUFFER_SIZE = 4096;
};

#endif /* !COMMUNICATION_HPP_ */
