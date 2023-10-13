#pragma once

#include "types.h"
#include "event_handler.h"

#include <vector>
#include <unordered_map>
#include <functional>

class BaseConnectionManager
{
public:
    ~BaseConnectionManager();
    virtual void sendMessage(const int fd, MessageType type, const std::vector<char> &message);
    virtual void sendMessage(const int fd, MessageType type, const void *data, size_t size);
    virtual void onDataReceived(const int fd, const char *data, ssize_t bytesRead);
    virtual void setHandler(EventHandler *eventHandler);
    virtual void resetHandler(EventHandler *callingEventHandler);
    virtual int getFd() const;

protected:
    struct Message
    {
        std::vector<uint8_t> buffer;
        int total_message_size = 0;
        bool size_known = false;
    };
    int connectionFd = -1; // the fd for the socket receiving connections or sending messages
    std::unordered_map<int, Message> clientBuffers;
    EventHandler* eventHandler = nullptr;

    virtual void disconnect(const int fd, const std::string &reason);

    virtual void onMessageReceived(const int fd, const uint8_t *data, const size_t size) = 0;
};
