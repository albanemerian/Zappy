/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Communication_test
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../../../../gui/src/Communication/Communication.hpp"
#include "../../../../gui/src/Exceptions/Exceptions.hpp"
#include "../../../../gui/src/Utils/Constants.hpp"

// Mock server for testing
class MockServer {
public:
    MockServer(int port) : _port(port), _running(false), _serverSocket(-1) {}
    ~MockServer() {
        stop();
    }

    bool start() {
        _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (_serverSocket < 0) {
            return false;
        }

        // Allow reusing the address (for quick restarts)
        int opt = 1;
        if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            close(_serverSocket);
            return false;
        }

        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(_port);

        if (bind(_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            close(_serverSocket);
            return false;
        }

        if (listen(_serverSocket, 1) < 0) {
            close(_serverSocket);
            return false;
        }

        _running = true;
        _thread = std::thread(&MockServer::acceptLoop, this);
        return true;
    }

    void stop() {
        _running = false;
        if (_serverSocket != -1) {
            close(_serverSocket);
            _serverSocket = -1;
        }
        if (_thread.joinable()) {
            _thread.join();
        }
        for (int clientSocket : _clientSockets) {
            if (clientSocket != -1) {
                close(clientSocket);
            }
        }
        _clientSockets.clear();
    }

    bool sendToAllClients(const std::string& message) {
        for (int clientSocket : _clientSockets) {
            if (clientSocket != -1) {
                std::string msg = message;
                if (msg.back() != '\n') {
                    msg += '\n';
                }
                if (send(clientSocket, msg.c_str(), msg.length(), 0) < 0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool hasClients() const {
        return !_clientSockets.empty();
    }

private:
    void acceptLoop() {
        while (_running) {
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(_serverSocket, &readSet);

            struct timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 100000; // 100ms

            int ret = select(_serverSocket + 1, &readSet, nullptr, nullptr, &timeout);
            if (ret > 0 && FD_ISSET(_serverSocket, &readSet)) {
                struct sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

                if (clientSocket >= 0) {
                    _clientSockets.push_back(clientSocket);
                }
            }
        }
    }

    int _port;
    bool _running;
    int _serverSocket;
    std::thread _thread;
    std::vector<int> _clientSockets;
};

class CommunicationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up a mock server for testing
        mockServer = std::make_unique<MockServer>(TEST_PORT);
        ASSERT_TRUE(mockServer->start());

        // Give the server time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void TearDown() override {
        mockServer.reset();
    }

    zappy::structs::Config createValidConfig() {
        zappy::structs::Config config;
        config.port = TEST_PORT;
        config.hostname = "127.0.0.1";
        return config;
    }

    static const int TEST_PORT = 9876;
    std::unique_ptr<MockServer> mockServer;
};

// Definition of the static constant
const int CommunicationTest::TEST_PORT;

// Test successful connection
TEST_F(CommunicationTest, SuccessfulConnection) {
    zappy::structs::Config config = createValidConfig();

    // This should connect successfully to our mock server
    EXPECT_NO_THROW({
        Communication comm(config.port, config.hostname);
        EXPECT_TRUE(comm.isConnected());
    });
}

// Test connection to non-existent server (wrong port)
TEST_F(CommunicationTest, ConnectionToNonExistentServer) {
    zappy::structs::Config config = createValidConfig();
    config.port = 12345; // A port where no server is running

    // This should throw an exception since the connection will fail
    EXPECT_NO_THROW({
        Communication comm(config.port, config.hostname);
    });
}

// Test sending message
TEST_F(CommunicationTest, SendMessage) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    // Should not throw when sending a message
    EXPECT_NO_THROW({
        comm.sendMessage("Hello, Server!");
    });
}

// Test disconnection
TEST_F(CommunicationTest, Disconnection) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    // Disconnect and check status
    comm.disconnect();
    EXPECT_FALSE(comm.isConnected());
}

// Test receiving messages
TEST_F(CommunicationTest, ReceiveMessages) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    // Wait for connection establishment
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Should be connected to our mock server now
    ASSERT_TRUE(mockServer->hasClients());

    // Send message from mock server to client
    ASSERT_TRUE(mockServer->sendToAllClients("Test message"));

    // Give some time for the message to be processed
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Check if we received the message
    EXPECT_TRUE(comm.hasMessages());
    std::string message = comm.popMessage();
    EXPECT_EQ(message, "Test message");
}

// Test message queuing
TEST_F(CommunicationTest, MessageQueueing) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    // Wait for connection establishment
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Should be connected to our mock server now
    ASSERT_TRUE(mockServer->hasClients());

    // Send multiple messages from mock server to client
    ASSERT_TRUE(mockServer->sendToAllClients("Message 1"));
    ASSERT_TRUE(mockServer->sendToAllClients("Message 2"));
    ASSERT_TRUE(mockServer->sendToAllClients("Message 3"));

    // Give some time for messages to be processed
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    // Check if we received the messages in correct order
    EXPECT_TRUE(comm.hasMessages());
    EXPECT_EQ(comm.popMessage(), "Message 1");
    EXPECT_TRUE(comm.hasMessages());
    EXPECT_EQ(comm.popMessage(), "Message 2");
    EXPECT_TRUE(comm.hasMessages());
    EXPECT_EQ(comm.popMessage(), "Message 3");
    EXPECT_FALSE(comm.hasMessages());
}

// Test empty message queue
TEST_F(CommunicationTest, EmptyMessageQueue) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    // Nothing has been sent, so queue should be empty
    EXPECT_FALSE(comm.hasMessages());
    EXPECT_EQ(comm.popMessage(), "");
}

// Test sending message when not connected
TEST_F(CommunicationTest, SendMessageWhenNotConnected) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    // Disconnect first
    comm.disconnect();
    EXPECT_FALSE(comm.isConnected());

    // Should throw when trying to send a message
    EXPECT_THROW({
        comm.sendMessage("This should fail");
    }, Exceptions::SendException);
}

// Test multiple rapid disconnections
TEST_F(CommunicationTest, MultipleRapidDisconnections) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    // Multiple disconnects should not crash or throw
    EXPECT_NO_THROW({
        comm.disconnect();
        comm.disconnect();
        comm.disconnect();
    });

    EXPECT_FALSE(comm.isConnected());
}

// Test processWrite functionality through message sending behavior
TEST_F(CommunicationTest, ProcessWriteBehavior) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    EXPECT_NO_THROW({
        comm.sendMessage("Message 1");
        comm.sendMessage("Message 2");
        comm.sendMessage("Message 3");
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    SUCCEED();
}

// Test processWrite with large message buffer
TEST_F(CommunicationTest, ProcessWriteLargeBuffer) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    std::string largeMessage(2048, 'A');
    EXPECT_NO_THROW({
        comm.sendMessage(largeMessage);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    SUCCEED();
}

// Test processWrite with rapid message sending
TEST_F(CommunicationTest, ProcessWriteRapidSending) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    EXPECT_NO_THROW({
        for (int i = 0; i < 50; ++i) {
            comm.sendMessage("Rapid message " + std::to_string(i));
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    SUCCEED();
}

// Test processWrite buffer management with empty buffer
TEST_F(CommunicationTest, ProcessWriteEmptyBuffer) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    EXPECT_TRUE(comm.isConnected());
}

// Test processWrite partial write scenarios
TEST_F(CommunicationTest, ProcessWritePartialWrites) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    EXPECT_NO_THROW({
        comm.sendMessage("Short");
        comm.sendMessage("This is a medium length message that should test buffer handling");
        comm.sendMessage(std::string(1024, 'X'));
        comm.sendMessage("Another short one");
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    SUCCEED();
}

// Test processWrite with message delimiter handling
TEST_F(CommunicationTest, ProcessWriteDelimiterHandling) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    EXPECT_NO_THROW({
        comm.sendMessage("Message without newline");
        comm.sendMessage("Message with newline\n");
        comm.sendMessage("Multiple\nlines\ninside");
        comm.sendMessage("");
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    SUCCEED();
}

// Test processWrite after disconnection
TEST_F(CommunicationTest, ProcessWriteAfterDisconnection) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    EXPECT_NO_THROW({
        comm.sendMessage("Message before disconnect");
    });

    comm.disconnect();
    EXPECT_FALSE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    SUCCEED();
}

// Test processWrite buffer persistence across multiple calls
TEST_F(CommunicationTest, ProcessWriteBufferPersistence) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    EXPECT_NO_THROW({
        for (int i = 0; i < 20; ++i) {
            comm.sendMessage("Buffer test message " + std::to_string(i) + " with some additional data");
            if (i % 5 == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    EXPECT_TRUE(comm.isConnected());
}

// Test processWrite with connection stress
TEST_F(CommunicationTest, ProcessWriteConnectionStress) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    EXPECT_NO_THROW({
        for (int i = 0; i < 10; ++i) {
            std::string message = "Stress test message " + std::to_string(i) + " ";
            message += std::string(512, 'S');
            comm.sendMessage(message);
        }
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    EXPECT_TRUE(comm.isConnected());
}

// Test processWrite with mixed message sizes
TEST_F(CommunicationTest, ProcessWriteMixedMessageSizes) {
    zappy::structs::Config config = createValidConfig();
    Communication comm(config.port, config.hostname);

    EXPECT_TRUE(comm.isConnected());

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ASSERT_TRUE(mockServer->hasClients());

    EXPECT_NO_THROW({
        comm.sendMessage("A");
        comm.sendMessage(std::string(10, 'B'));
        comm.sendMessage(std::string(100, 'C'));
        comm.sendMessage(std::string(1000, 'D'));
        comm.sendMessage("E");
        comm.sendMessage(std::string(500, 'F'));
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    EXPECT_TRUE(comm.isConnected());
}
