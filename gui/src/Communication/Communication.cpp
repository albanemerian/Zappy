/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Communication
*/

#include <netdb.h>
#include <iostream>
#include <cstring>
#include <cerrno>
#include <sstream>
#include <string>

#include "Communication.hpp"

Communication::Communication(int port, const std::string &hostname,
                             const std::string &delimiter)
    : _port(port),
      _hostname(hostname),
      _delimiter(delimiter.empty() ? "\n" : delimiter),
      _running(false),
      _connected(false),
      _receiveBuffer(""),
      _sendBuffer(""),
      _socket(-1)
{
    try {
        setupConnection();
        startCommunicationThread();
    } catch (const Exceptions::NetworkException &e) {
        std::cerr << e.what() << std::endl;
        disconnect();
    }
}

Communication::~Communication()
{
    disconnect();
}

void Communication::sendMessage(const std::string &message)
{
    if (!_connected)
        throw Exceptions::SendException("Not connected to server");

    std::lock_guard<std::mutex> lock(_mutex);
    _outgoingMessages.push(message);
    _cv.notify_one();
}

bool Communication::hasMessages() const
{
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(_mutex));
    return !_incomingMessages.empty();
}

std::string Communication::popMessage()
{
    std::lock_guard<std::mutex> lock(_mutex);

    if (_incomingMessages.empty())
        return "";

    std::string message = _incomingMessages.front();
    _incomingMessages.pop();
    return message;
}

bool Communication::isConnected() const
{
    return _connected;
}

void Communication::disconnect()
{
    _running = false;

    if (_thread.joinable())
        _thread.join();

    if (_socket != -1) {
        close(_socket);
        _socket = -1;
    }

    _connected = false;
}

void Communication::setupConnection()
{
    createSocket();
    connectToServer();
    setupNonBlocking();

    _pollfd.fd = _socket;
    _pollfd.events = POLLIN | POLLOUT;

    _connected = true;
    std::cout << colors::T_GREEN << "Successfully connected to "
              << _hostname << ":" << _port
              << colors::RESET << std::endl;
}

void Communication::createSocket()
{
    _socket = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket < 0) {
        std::string errorMsg = "Failed to create socket: ";
        errorMsg += strerror(errno);
        throw Exceptions::SocketCreationException(errorMsg);
    }
}

void Communication::connectToServer()
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(_port);

    if (inet_pton(AF_INET, _hostname.c_str(), &serverAddr.sin_addr) <= 0) {
        std::string errorMsg = "Invalid address: ";
        errorMsg += strerror(errno);
        close(_socket);
        throw Exceptions::ConnectionFailedException(errorMsg);
    }

    if (connect(_socket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::string errorMsg = "Connection failed: ";
        errorMsg += strerror(errno);
        close(_socket);
        throw Exceptions::ConnectionFailedException(errorMsg);
    }
}

void Communication::setupNonBlocking()
{
    int flags = fcntl(_socket, F_GETFL, 0);

    if (flags == -1) {
        std::string errorMsg = "Failed to get socket flags: ";
        errorMsg += strerror(errno);
        close(_socket);
        throw Exceptions::SocketCreationException(errorMsg);
    }

    if (fcntl(_socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::string errorMsg = "Failed to set socket non-blocking: ";
        errorMsg += strerror(errno);
        close(_socket);
        throw Exceptions::SocketCreationException(errorMsg);
    }
}

void Communication::startCommunicationThread()
{
    _running = true;
    _thread = std::thread(&Communication::communicationLoop, this);
}

void Communication::communicationLoop()
{
    while (_running) {
        {
            std::lock_guard<std::mutex> lock(_mutex);

            if (!_outgoingMessages.empty()) {
                std::string &message = _outgoingMessages.front();
                _sendBuffer += message;

                if (!message.empty() && !_delimiter.empty() &&
                    (message.size() < _delimiter.size() ||
                    message.substr(message.size() - _delimiter.size()) != _delimiter))
                    _sendBuffer += _delimiter;

                _outgoingMessages.pop();
            }
        }

        try {
            if (!handlePoll()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            }
        } catch (const Exceptions::NetworkException &e) {
            std::cerr << e.what() << std::endl;
            _connected = false;
            break;
        }
    }
}

bool Communication::handlePoll()
{
    int ret = poll(&_pollfd, 1, -1);

    if (ret < 0) {
        std::string errorMsg = "Poll failed: ";
        errorMsg += strerror(errno);
        throw Exceptions::NetworkException(errorMsg);
    }

    if (ret == 0)
        return false;

    if (_pollfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
        std::string errorMsg = "Socket error: ";
        if (_pollfd.revents & POLLERR)
            errorMsg += "Error condition ";
        if (_pollfd.revents & POLLHUP)
            errorMsg += "Hung up ";
        if (_pollfd.revents & POLLNVAL)
            errorMsg += "Invalid request ";
        throw Exceptions::NetworkException(errorMsg);
    }

    if (_pollfd.revents & POLLIN)
        processRead();

    if (_pollfd.revents & POLLOUT && !_sendBuffer.empty())
        processWrite();

    return true;
}

void Communication::processWrite()
{
    if (_sendBuffer.empty())
        return;

    int bytesWritten = write(_socket, _sendBuffer.c_str(), _sendBuffer.size());

    if (bytesWritten < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;

        std::string errorMsg = "Write failed: ";
        errorMsg += strerror(errno);
        throw Exceptions::SendException(errorMsg);
    }

    _sendBuffer.erase(0, bytesWritten);
}

void Communication::processRead()
{
    char buffer[BUFFER_SIZE];
    int bytesRead = read(_socket, buffer, BUFFER_SIZE - 1);

    if (bytesRead < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;

        std::string errorMsg = "Read failed: ";
        errorMsg += strerror(errno);
        throw Exceptions::ReceiveException(errorMsg);
    }

    if (bytesRead == 0)
        throw Exceptions::ConnectionFailedException("Server closed connection");

    buffer[bytesRead] = '\0';
    _receiveBuffer += buffer;

    parseReceivedData();
}

void Communication::parseReceivedData()
{
    size_t pos;

    while ((pos = _receiveBuffer.find(_delimiter)) != std::string::npos) {
        std::string message = _receiveBuffer.substr(0, pos);
        _receiveBuffer.erase(0, pos + _delimiter.length());

        std::lock_guard<std::mutex> lock(_mutex);

        _incomingMessages.push(message);
    }
}
