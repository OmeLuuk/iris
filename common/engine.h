#pragma once

#include "base_connection_manager.h"
#include "event_handler.h"

#include <chrono>

class Engine : public EventHandler
{
public:
    Engine(int port, BaseConnectionManager &manager);
    ~Engine();

    void ReactiveEventLoop();
    void EventLoop(const std::chrono::milliseconds waitTime);
    void Disconnect(int fd);

protected:
    virtual void onMessage(int fd, const void *data, size_t size) override;
    virtual void onConnected(const int fd) override;
    virtual void onDisconnected(const int fd) override;
    virtual void EventCycle();
    int server_fd;
    int epoll_fd;
    BaseConnectionManager &connectionManager;

private:
    static constexpr std::chrono::milliseconds defaultWaitTime = std::chrono::milliseconds(0);
};