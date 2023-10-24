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
    void onMessage(const MessageType type, int client_fd, const void *data, size_t size) override;
    void addSubscriber(int client_fd, const void *data, size_t size);
    void broadcastMessage(const void *data, size_t size);
    void onConnected(const int client_fd, const ClientType type) override;
    void onDisconnected(const int fd) override;

    std::unordered_map<int, ClientType> fdToClientType;
    std::unordered_set<int> broadcastConsumers;
    std::unordered_map<std::string, std::unordered_set<int>> topicSubscribers;
};