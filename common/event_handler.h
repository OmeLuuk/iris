#pragma once

#include <cstring>

class EventHandler
{
public:
    virtual void onMessage(int fd, const void *data, size_t size) = 0;
    virtual void onConnected(const int fd, const ClientType type) = 0;
    virtual void onDisconnected(const int fd) = 0;
};