#pragma once

#include "types.h"

#include <vector>
#include <unordered_map>
#include <functional>

class BaseConnectionHandler
{
public:
    using MessageCallback = std::function<void(int, const void *, size_t)>;
    using ConnectionCallback = std::function<void(const int)>;

    void sendMessage(MessageType type, const std::vector<char> &message);
    void onDataReceived(const int client_fd, const char *data, ssize_t bytesRead);

protected:
    struct Message
    {
        std::vector<uint8_t> buffer;
        int total_message_size = 0;
        bool size_known = false;
    };
    int sockfd;
    std::unordered_map<int, Message> clientBuffers;

    void disconnect(const int client_fd, const std::string &reason);

    virtual void onMessageReceived(const int client_fd, const uint8_t *data, const size_t size) = 0;
};
