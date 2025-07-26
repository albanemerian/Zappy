/*
** EPITECH PROJECT, 2025
** B-YEP-400-NAN-4-1-zappy-albane.merian
** File description:
** CLI_test
*/

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../../../../gui/src/CLI/CLI.hpp"
#include "../../../../gui/src/Exceptions/Exceptions.hpp"

class CLITest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    char **createArgv(const std::vector<std::string> &args) {
        char **argv = new char*[args.size()];
        for (size_t i = 0; i < args.size(); i++) {
            argv[i] = new char[args[i].length() + 1];
            strcpy(argv[i], args[i].c_str());
        }
        return argv;
    }

    void cleanupArgv(char **argv, int argc) {
        for (int i = 0; i < argc; i++) {
            delete[] argv[i];
        }
        delete[] argv;
    }
};

// Test valid arguments parsing
TEST_F(CLITest, ValidArgumentParsing) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    auto config = cli.parseArguments(argc, argv);

    EXPECT_EQ(config.port, 4242);
    EXPECT_EQ(config.hostname, "127.0.0.1");

    cleanupArgv(argv, argc);
}

// Test with localhost conversion
TEST_F(CLITest, LocalhostConversion) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "localhost"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    auto config = cli.parseArguments(argc, argv);

    EXPECT_EQ(config.port, 4242);
    EXPECT_EQ(config.hostname, "127.0.0.1");

    cleanupArgv(argv, argc);
}

// Test reversed argument order
TEST_F(CLITest, ReversedArgumentOrder) {
    std::vector<std::string> args = {"./zappy_gui", "-h", "127.0.0.1", "-p", "4242"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    auto config = cli.parseArguments(argc, argv);

    EXPECT_EQ(config.port, 4242);
    EXPECT_EQ(config.hostname, "127.0.0.1");

    cleanupArgv(argv, argc);
}

// Test incorrect number of arguments
TEST_F(CLITest, IncorrectNumberOfArguments) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIInvalidArgumentException);

    cleanupArgv(argv, argc);
}

// Test missing port argument
TEST_F(CLITest, MissingPortArgument) {
    std::vector<std::string> args = {"./zappy_gui", "-h", "127.0.0.1", "-h", "127.0.0.1",};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIMissingArgumentException);

    cleanupArgv(argv, argc);
}

// Test missing hostname argument
TEST_F(CLITest, MissingHostnameArgument) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-p", "4242"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIMissingArgumentException);

    cleanupArgv(argv, argc);
}

// Test missing port flag
TEST_F(CLITest, MissingPortFlag) {
    std::vector<std::string> args = {"./zappy_gui", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIInvalidArgumentException);

    cleanupArgv(argv, argc);
}

// Test missing hostname flag
TEST_F(CLITest, MissingHostnameFlag) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIInvalidArgumentException);

    cleanupArgv(argv, argc);
}

// Test invalid port (negative)
TEST_F(CLITest, InvalidPortNegative) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "-4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIPortException);

    cleanupArgv(argv, argc);
}

// Test invalid port (zero)
TEST_F(CLITest, InvalidPortZero) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "0", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIPortException);

    cleanupArgv(argv, argc);
}

// Test invalid port (too large)
TEST_F(CLITest, InvalidPortTooLarge) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "65536", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIPortException);

    cleanupArgv(argv, argc);
}

// Test invalid port (Doesn't fit in int)
TEST_F(CLITest, InvalidPortDoesntFitInInt) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "999999999999", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIPortException);

    cleanupArgv(argv, argc);
}

// Test invalid port (non-numeric)
TEST_F(CLITest, InvalidPortNonNumeric) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "port", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIPortException);

    cleanupArgv(argv, argc);
}

// Test maximum valid port
TEST_F(CLITest, MaximumValidPort) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "65535", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    auto config = cli.parseArguments(argc, argv);

    EXPECT_EQ(config.port, 65535);
    EXPECT_EQ(config.hostname, "127.0.0.1");

    cleanupArgv(argv, argc);
}

// Test invalid IP address (empty)
TEST_F(CLITest, InvalidIPEmpty) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", ""};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIHostException);

    cleanupArgv(argv, argc);
}

// Test invalid IP address (bad format - too few segments)
TEST_F(CLITest, InvalidIPTooFewSegments) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIHostException);

    cleanupArgv(argv, argc);
}

// Test invalid IP address (bad format - too many segments)
TEST_F(CLITest, InvalidIPTooManySegments) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.1.5"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIHostException);

    cleanupArgv(argv, argc);
}

// Test invalid IP address (out of range values)
TEST_F(CLITest, InvalidIPOutOfRange) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.256"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIHostException);

    cleanupArgv(argv, argc);
}

// Test invalid IP address (non-numeric segments)
TEST_F(CLITest, InvalidIPNonNumericSegments) {
    std::vector<std::string> args = {"./zappy_gui", "-p", "4242", "-h", "127.0.0.abc"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIHostException);

    cleanupArgv(argv, argc);
}

// Test unknown argument flag
TEST_F(CLITest, UnknownArgumentFlag) {
    std::vector<std::string> args = {"./zappy_gui", "unknown", "4242", "-h", "127.0.0.1"};
    int argc = args.size();
    char **argv = createArgv(args);

    CLI cli(argc, argv);
    EXPECT_THROW(cli.parseArguments(argc, argv), Exceptions::CLIInvalidArgumentException);

    cleanupArgv(argv, argc);
}
