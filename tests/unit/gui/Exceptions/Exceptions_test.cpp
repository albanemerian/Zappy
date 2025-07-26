/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** Exceptions_test
*/

#include <gtest/gtest.h>
#include <string>
#include "../../../../gui/src/Exceptions/Exceptions.hpp"

class ExceptionsTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test the base CLIParsingException
TEST_F(ExceptionsTest, CLIParsingExceptionTest) {
    std::string errorMessage = "Test error message";
    Exceptions::CLIParsingException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "CLI Parsing Error: " + errorMessage +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const std::exception*>(&exception) != nullptr);
}

// Test the CLIPortException
TEST_F(ExceptionsTest, CLIPortExceptionTest) {
    std::string errorMessage = "Port must be a positive integer";
    Exceptions::CLIPortException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "CLI Parsing Error: " +
                                std::string(colors::T_CYAN) +
                                "Port Error: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::CLIParsingException*>(&exception) != nullptr);
}

// Test the CLIHostException
TEST_F(ExceptionsTest, CLIHostExceptionTest) {
    std::string errorMessage = "Hostname cannot be empty";
    Exceptions::CLIHostException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "CLI Parsing Error: " +
                                std::string(colors::T_CYAN) +
                                "Hostname Error: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::CLIParsingException*>(&exception) != nullptr);
}

// Test the CLIMissingArgumentException
TEST_F(ExceptionsTest, CLIMissingArgumentExceptionTest) {
    std::string errorMessage = "Missing required option: -p port";
    Exceptions::CLIMissingArgumentException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "CLI Parsing Error: " +
                                std::string(colors::T_CYAN) +
                                "Missing Argument: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::CLIParsingException*>(&exception) != nullptr);
}

// Test the CLIInvalidArgumentException
TEST_F(ExceptionsTest, CLIInvalidArgumentExceptionTest) {
    std::string errorMessage = "Unknown option: -x";
    Exceptions::CLIInvalidArgumentException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "CLI Parsing Error: " +
                                std::string(colors::T_CYAN) +
                                "Invalid Argument: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::CLIParsingException*>(&exception) != nullptr);
}

// Test the base NetworkException
TEST_F(ExceptionsTest, NetworkExceptionTest) {
    std::string errorMessage = "General network error";
    Exceptions::NetworkException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "Network Error: " + errorMessage +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const std::exception*>(&exception) != nullptr);
}

// Test the ConnectionFailedException
TEST_F(ExceptionsTest, ConnectionFailedExceptionTest) {
    std::string errorMessage = "Could not connect to host";
    Exceptions::ConnectionFailedException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "Network Error: " +
                                std::string(colors::T_CYAN) +
                                "Connection Failed: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::NetworkException*>(&exception) != nullptr);
}

// Test the SocketCreationException
TEST_F(ExceptionsTest, SocketCreationExceptionTest) {
    std::string errorMessage = "Failed to create socket";
    Exceptions::SocketCreationException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "Network Error: " +
                                std::string(colors::T_CYAN) +
                                "Socket Creation Failed: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::NetworkException*>(&exception) != nullptr);
}

// Test the ConnectionTimeoutException
TEST_F(ExceptionsTest, ConnectionTimeoutExceptionTest) {
    std::string errorMessage = "Connection attempt timed out";
    Exceptions::ConnectionTimeoutException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "Network Error: " +
                                std::string(colors::T_CYAN) +
                                "Connection Timeout: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::NetworkException*>(&exception) != nullptr);
}

// Test the SendException
TEST_F(ExceptionsTest, SendExceptionTest) {
    std::string errorMessage = "Failed to send data";
    Exceptions::SendException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "Network Error: " +
                                std::string(colors::T_CYAN) +
                                "Send Error: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::NetworkException*>(&exception) != nullptr);
}

// Test the ReceiveException
TEST_F(ExceptionsTest, ReceiveExceptionTest) {
    std::string errorMessage = "Failed to receive data";
    Exceptions::ReceiveException exception(errorMessage);

    std::string expectedMessage = std::string(colors::T_RED) +
                                "Network Error: " +
                                std::string(colors::T_CYAN) +
                                "Receive Error: " + errorMessage +
                                colors::RESET +
                                colors::RESET;

    EXPECT_STREQ(exception.what(), expectedMessage.c_str());
    EXPECT_TRUE(dynamic_cast<const Exceptions::NetworkException*>(&exception) != nullptr);
}

// Test the ModuleException
TEST_F(ExceptionsTest, ModuleExceptionTest) {
    std::string errorMessage = "Module initialization failed";
    Exceptions::ModuleAudioError exception(errorMessage);

    EXPECT_STREQ(exception.what(), exception.what());
    EXPECT_TRUE(dynamic_cast<const Exceptions::ModuleAudioError*>(&exception) != nullptr);
}
