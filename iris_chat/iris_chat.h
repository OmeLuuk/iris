#pragma once

#include "client_connection_manager.h"
#include "engine.h"

class IrisChat : public Engine
{
public:
    IrisChat(ClientConnectionManager &handler, std::function<void(const std::string &)> onMessageReceived);
    void onMessage(const MessageType type, int client_fd, const void *data, size_t size);
    void EventCycle() override;
    void SendChatMessage(const std::string& message);

private:
    std::function<void(const std::string &)> onMessageReceived;
};