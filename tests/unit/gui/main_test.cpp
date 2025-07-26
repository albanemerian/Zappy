/*
** EPITECH PROJECT, 2025
** B-CCP-400-NAN-4-1-theplazza-eliott.tesnier
** File description:
** Unit test runner
*/

#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

class OutputRedirector {
public:
    OutputRedirector() {
        originalCout = std::cout.rdbuf();
        originalCerr = std::cerr.rdbuf();

        std::cout.rdbuf(&nullBuffer);
        std::cerr.rdbuf(&nullBuffer);
    }

    ~OutputRedirector() {
        std::cout.rdbuf(originalCout);
        std::cerr.rdbuf(originalCerr);
    }

private:
    std::streambuf* originalCout;
    std::streambuf* originalCerr;

    class NullBuffer : public std::streambuf {
        protected:
            int overflow(int c) override {
                return c == EOF ? EOF : c;
            }
    };
    NullBuffer nullBuffer;
};

int main(int argc, char **argv)
{
    OutputRedirector redirector;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
