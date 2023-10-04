#pragma once

#include "server_connection_manager.h"
#include "engine.h"

#include <unistd.h>
#include <string>

class IrisServer : public Engine
{
public:
    IrisServer(int port, ServerConnectionManager &handler);
    ~IrisServer();

private:
    void onMessage(int client_fd, const void *data, size_t size) override;
    void onConnected(const int client_fd) override;
};