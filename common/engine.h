#pragma once

#include "base_connection_manager.h"
#include "event_handler.h"

class Engine : public EventHandler
{
public:
    Engine(int port, BaseConnectionManager &manager);
    ~Engine();

    void EventLoop();
    void Disconnect(int fd);

    virtual void onMessage(int fd, const void *data, size_t size) override;
    virtual void onConnected(const int fd) override;
    virtual void onDisconnected(const int fd) override;

protected:
    int server_fd;
    int epoll_fd;
    BaseConnectionManager &connectionManager;
};