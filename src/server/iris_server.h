#pragma once

#include "connection_handler.h"

#include <unistd.h>
#include <string>

class IrisServer
{
public:
    IrisServer(int port);
    ~IrisServer();
    void spin();

private:
    void handleMessage(const int client_fd, char *buffer, ssize_t bytesRead);
    void handleDataMessage(const int client_fd, char *buffer, ssize_t bytesRead);
    void handleIntroMessage(const int client_fd, char *buffer, ssize_t bytesRead);
    void disconnectClient(int client_fd, const std::string &reason);
    void onMessage(int client_fd, const void *data, size_t size);
    void onConnected(const int client_fd);
    void onDisconnected(const int client_fd);

    int server_fd;
    int epoll_fd;

    ConnectionHandler connectionHandler;
};