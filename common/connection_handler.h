#pragma once

#include "types.h"

#include <string>
#include <functional>
#include <cstddef>
#include <cstdint>
#include <netinet/in.h>
#include <unistd.h>

class ConnectionHandler
{
public:
    using MessageCallback = std::function<void(int, const void*, size_t)>;
    using ConnectionCallback = std::function<void(const int)>;

    ConnectionHandler(MessageCallback onMessage, ConnectionCallback onConnect, ConnectionCallback onDisconnect);

    void onDataReceived(const int client_fd, const char* data, ssize_t size);
    void onConnected(int client_fd);
    void onDisconnected(int client_fd);

private:
    struct Message
    {
        std::vector<uint8_t> buffer;
        int total_message_size = 0;
        bool size_known = false;
    };

    void onMessageReceived(const int client_fd, const uint8_t* data, size_t size);
    void disconnect(const int client_fd, const std::string& reason);

    MessageCallback onMessageCallback;
    ConnectionCallback onConnectedCallback;
    ConnectionCallback onDisconnectedCallback;

    std::unordered_map<int, Message> clientBuffers;
};
