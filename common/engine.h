#pragma once

#include "base_connection_manager.h"
#include "event_handler.h"

#include <chrono>

class Engine : public EventHandler
{
public:
    Engine(bool acceptConnections, BaseConnectionManager &manager);
    Engine(BaseConnectionManager &connectionManager);
    ~Engine();

    void ReactiveEventLoop();
    void EventLoop(const std::chrono::milliseconds waitTime);
    void Disconnect(int fd);

protected:
    virtual void onMessage(int fd, const void *data, size_t size) override {};
    virtual void onConnected(const int fd, const ClientType type) override {};
    void onDisconnectedEvent(const int fd) override final;
    virtual void onDisconnected(const int fd) {};

    virtual void EventCycle();
    void sendMessage(const int fd, const MessageType type, const std::vector<char> &msg);
    void sendMessage(const int fd, const MessageType type, const void *data, size_t size);
    int epoll_fd = 0;
    const bool canAcceptConnections = false;
    BaseConnectionManager &connectionManager;
};