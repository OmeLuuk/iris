#include "logging.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

void log(LL level, const std::string &message)
{
    if constexpr (!enableDebugLogging && (level == LL::DEBUG))
        return;

    const auto now = std::chrono::system_clock::now();
    const auto now_time_t = std::chrono::system_clock::to_time_t(now);
    const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
    char timestamp[15];                                                                   // Adjusted buffer size
    std::strftime(timestamp, sizeof(timestamp), "%H:%M:%S", std::localtime(&now_time_t)); // Remove date, keep time

    const char *colorReset = "\033[0m";
    const char *levelStr;
    const char *colorCode;
    std::ostream &stream = (level == LL::ERROR) ? std::cerr : std::cout;

    switch (level)
    {
    case LL::INFO:
        levelStr = "INFO ";
        colorCode = "";
        break;
    case LL::DEBUG:
        levelStr = "DEBUG";
        colorCode = "";
        break;
    case LL::ERROR:
        levelStr = "ERROR";
        colorCode = "\033[31m";
        break;
    }

    stream << "[" << timestamp << "." << std::setw(9) << std::setfill('0') << ns.count()
           << "] [" << colorCode << levelStr << colorReset << "] " << message << std::endl;
}
