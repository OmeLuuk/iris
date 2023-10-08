#pragma once

#include <string>

#ifdef NDEBUG
constexpr bool enableDebugLogging = false;
#else
constexpr bool enableDebugLogging = true;
#endif

enum class LL
{
    DEBUG,
    INFO,
    ERROR
};

void log(LL level, const void *data, size_t size);
void log(LL level, const std::string &message);
