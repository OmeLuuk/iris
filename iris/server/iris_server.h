#pragma once

#include "server_connection_manager.h"
#include "engine.h"

#include <unistd.h>
#include <string>
#include <unordered_set>

class IrisServer : public Engine
{
public:
    IrisServer(ServerConnectionManager &handler);
    ~IrisServer();

private:
    void handleMessage(const SubscribeMessage& message, const int fd) override;
    void handleMessage(const PublicMessage &message, const int fd) override;
    void handleMessage(const UserUpdate &message, const int fd) override;

    void onConnected(const int client_fd, const ClientType type) override;
    void onDisconnected(const int fd) override;

    std::unordered_map<int, ClientType> fdToClientType;
    std::unordered_map<ClientType, std::unordered_set<int>> clientTypeToFds;
    std::unordered_map<int, std::string> fdToIrisChatClient;
    std::unordered_set<int> broadcastConsumers;
    std::unordered_map<std::string, std::unordered_set<int>> topicSubscribers;
};