#pragma once

#include "types.h"
#include "base_connection_manager.h"

#include <string>
#include <cstddef>
#include <cstdint>
#include <netinet/in.h>
#include <unistd.h>

class ServerConnectionManager : public BaseConnectionManager
{
public:
    ServerConnectionManager();

    void onDisconnected(int client_fd);

private:
    void onMessageReceived(const int client_fd, const uint8_t* data, const size_t size) override;
    void handleErrorMessage(const int client_fd, const uint8_t *data, const size_t size);
    void handleIntroMessage(const IntroMessage &message, const int client_fd);
    void handleDataMessage(const MessageType type, const int client_fd, const uint8_t *data, const size_t size);
};
