#include "logging.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

void log(LL level, const void *data, size_t size)
{
    if (!enableDebugLogging && (level == LL::DEBUG))
        return;

    const char *byte_data = static_cast<const char *>(data);
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < size; ++i)
    {
        ss << static_cast<unsigned char>(byte_data[i]);
    }

    log(level, ss.str());
}

void log(LL level, const std::string &message)
{
    log(level, "%s", message.c_str()); 
}