#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>

enum class LL
{
    INFO,
    ERROR
};

inline void log(LL level, const std::string &message)
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
    char timestamp[15];                                                                   // Adjusted buffer size
    std::strftime(timestamp, sizeof(timestamp), "%H:%M:%S", std::localtime(&now_time_t)); // Removed date, kept time

    const char *colorReset = "\033[0m";
    const char *levelStr;
    const char *colorCode;
    std::ostream &stream = (level == LL::INFO) ? std::cout : std::cerr;

    if (level == LL::INFO)
    {
        levelStr = "INFO ";
        colorCode = ""; // No color code for INFO
    }
    else
    {
        levelStr = "ERROR";
        colorCode = "\033[31m"; // Red for ERROR
    }

    stream << "[" << timestamp << "." << std::setw(9) << std::setfill('0') << ns.count()
           << "] [" << colorCode << levelStr << colorReset << "] " << message << std::endl;
}
