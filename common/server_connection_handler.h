#pragma once

#include "types.h"
#include "base_connection_handler.h"

#include <string>
#include <cstddef>
#include <cstdint>
#include <netinet/in.h>
#include <unistd.h>

class ServerConnectionHandler : public BaseConnectionHandler
{
public:
    ServerConnectionHandler(MessageCallback onMessage, ConnectionCallback onConnect, ConnectionCallback onDisconnect);

    void onConnected(int client_fd);
    void onDisconnected(int client_fd);

private:
    void onMessageReceived(const int client_fd, const uint8_t* data, const size_t size) override;
    void handleErrorMessage(const int client_fd, const uint8_t *data, const size_t size);
    void handleIntroMessage(const int client_fd, const uint8_t* data, const size_t size);
    void handleDataMessage(const int client_fd, const uint8_t *data, const size_t size);

    MessageCallback onMessageCallback;
    ConnectionCallback onConnectedCallback;
    ConnectionCallback onDisconnectedCallback;
};
