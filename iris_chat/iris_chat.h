#pragma once

#include "client_connection_manager.h"
#include "engine.h"

class IrisChat : public Engine
{
public:
    IrisChat(ClientConnectionManager &handler);
    void onMessage(const MessageType type, int client_fd, const void *data, size_t size);

private:
    void EventCycle() override;
};