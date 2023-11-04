#pragma once

#include "client_connection_manager.h"
#include "engine.h"

using onMessageReceivedCallback = std::function<void(const std::string &, const std::string &, const std::string &)>;

class IrisChat : public Engine
{
public:
    IrisChat(ClientConnectionManager &handler, onMessageReceivedCallback onMessageReceived, const std::string &username);
    void onMessage(const MessageType type, int client_fd, const void *data, size_t size);
    void EventCycle() override;
    void SendChatMessage(const std::string& message);

private:
    onMessageReceivedCallback onMessageReceived;
    const std::string& username;
};