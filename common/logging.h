#pragma once

#include <string>

enum class LL
{
    INFO,
    ERROR
};

void log(LL level, const std::string &message);
