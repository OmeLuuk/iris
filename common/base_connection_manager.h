#pragma once

#include "types.h"
#include "event_handler.h"

#include <vector>
#include <unordered_map>
#include <functional>

class BaseConnectionManager
{
public:
    virtual void sendMessage(MessageType type, const std::vector<char> &message);
    virtual void onDataReceived(const int client_fd, const char *data, ssize_t bytesRead);
    virtual void setHandler(EventHandler *eventHandler);
    virtual void resetHandler(EventHandler *callingEventHandler);

protected:
    struct Message
    {
        std::vector<uint8_t> buffer;
        int total_message_size = 0;
        bool size_known = false;
    };
    int sockfd;
    std::unordered_map<int, Message> clientBuffers;
    EventHandler* eventHandler = nullptr;

    virtual void disconnect(const int client_fd, const std::string &reason);

    virtual void onMessageReceived(const int client_fd, const uint8_t *data, const size_t size) = 0;
};
