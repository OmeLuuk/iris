#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

#ifdef NDEBUG
constexpr bool enableDebugLogging = false;
#else
constexpr bool enableDebugLogging = true;
#endif

namespace
{
    template <typename Arg, typename... Args>
    void customFormat(std::ostream &stream, const char *fmt, const Arg &arg, const Args &...args)
    {
        for (; *fmt != '\0'; ++fmt)
        {
            if (*fmt == '%' && *(++fmt) != '%')
            {
                stream << arg;
                customFormat(stream, fmt + 1, args...); // Recur with one fewer argument
                return;
            }
            stream << *fmt;
        }
        throw std::runtime_error("extra arguments provided to printf");
    }

    // Terminal print for the base case
    void customFormat(std::ostream &stream, const char *fmt)
    {
        for (; *fmt != '\0'; ++fmt)
        {
            if (*fmt == '%' && *(++fmt) != '%')
            {
                throw std::runtime_error("invalid format string: missing arguments");
            }
            stream << *fmt;
        }
    }
}

enum class LL
{
    DEBUG,
    INFO,
    ERROR
};

void log(LL level, const void *data, size_t size);
void log(LL level, const std::string &message);

template <typename... Args>
void log(LL level, const char *fmt, const Args &...args)
{
    if (!enableDebugLogging && (level == LL::DEBUG))
        return;

    const auto now = std::chrono::system_clock::now();
    const auto now_time_t = std::chrono::system_clock::to_time_t(now);
    const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()) % 1000000000;
    char timestamp[15];
    std::strftime(timestamp, sizeof(timestamp), "%H:%M:%S", std::localtime(&now_time_t));

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

    // Print the log message directly to the output stream
    stream << "[" << timestamp << "." << std::setw(9) << std::setfill('0') << ns.count()
           << "] [" << colorCode << levelStr << colorReset << "] ";
    customFormat(stream, fmt, args...);
    stream << std::endl;
}
