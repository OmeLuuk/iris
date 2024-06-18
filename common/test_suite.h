#pragma once
#include <string>

namespace
{
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";
}

template <typename T>
void EQUALS(const std::string &testName, const T &expected, const T &actual)
{
    if (expected == actual)
    {
        std::cout << GREEN << "PASS: " << testName << RESET << std::endl;
    }
    else
    {
        std::cout << RED << "FAIL: " << testName << RESET << std::endl;
        std::cout << "Expected:\t" << expected << std::endl;
        std::cout << "Actual:\t\t" << actual << std::endl;
    }
}
