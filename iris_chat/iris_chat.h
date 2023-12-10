#pragma once

#include "client_connection_manager.h"
#include "engine.h"

using OnMessageReceivedCallback = std::function<void(const PublicMessage &)>;
using OnUserStatusUpdateCallback = std::function<void(const UserUpdate &)>;

class IrisChat : public Engine
{
public:
    IrisChat(ClientConnectionManager &handler, OnMessageReceivedCallback onMessageReceived, const std::string &username, OnUserStatusUpdateCallback onUserStatusChanged);
    void onMessage(const MessageType type, int client_fd, const void *data, size_t size);
    void handleMessage(const PublicMessage &message);
    void EventCycle() override;
    void SendChatMessage(const std::string &topic, const std::string &message);
    void SubscribeToTopic(const std::string &topic);

private:
    OnMessageReceivedCallback onMessageReceived;
    OnUserStatusUpdateCallback onUserStatusChanged;
    const std::string &username;
};