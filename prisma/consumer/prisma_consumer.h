#pragma once

#include "client_connection_manager.h"
#include "engine.h"

#include <unistd.h>
#include <string>

class PrismaConsumer: public Engine
{
public:
    PrismaConsumer(ClientConnectionManager &handler);
    ~PrismaConsumer();

private:
    void onMessage(const MessageType type, int client_fd, const void *data, size_t size) override;
};