#pragma once

#include <cstring>

class EventHandler
{
public:
    virtual ~EventHandler() = default;
    virtual void onMessage(const MessageType messageType, const int fd, const void *data, size_t size) = 0;
    virtual void onConnected(const int fd, const ClientType type) = 0;
    virtual void onDisconnectedEvent(const int fd) = 0;
};