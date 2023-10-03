#pragma once

#include "server_connection_handler.h"

#include <unistd.h>
#include <string>

class IrisServer
{
public:
    IrisServer(int port);
    ~IrisServer();
    void spin();

private:
    void onMessage(int client_fd, const void *data, size_t size);
    void onConnected(const int client_fd);
    void onDisconnected(const int client_fd);

    int server_fd;
    int epoll_fd;

    ServerConnectionHandler connectionHandler;
};