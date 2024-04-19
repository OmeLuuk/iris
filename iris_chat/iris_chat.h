#pragma once

#include "client_connection_manager.h"
#include "engine.h"

using OnMessageReceivedCallback = std::function<void(const PublicMessage &)>;
using OnUserStatusUpdateCallback = std::function<void(const UserUpdate &)>;

class IrisChat : public Engine
{
public:
    IrisChat(ClientConnectionManager &handler, OnMessageReceivedCallback onMessageReceived, const std::string &username, OnUserStatusUpdateCallback onUserStatusChanged);
    void handleMessage(const PublicMessage &message, const int fd) override;
    void handleMessage(const UserUpdate &message, const int fd) override;
    void EventCycle() override;
    void SendChatMessage(const std::string &topic, const std::string &message);
    void SubscribeToTopic(const std::string &topic);

private:
    OnMessageReceivedCallback onMessageReceived;
    OnUserStatusUpdateCallback onUserStatusChanged;
    const std::string &username;
};