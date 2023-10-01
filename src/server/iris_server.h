#pragma once

#include <unistd.h>

class IrisServer
{
public:
    IrisServer(int port);
    ~IrisServer();
    void spin();

private:
    bool handleMessage(char *buffer, ssize_t bytesRead);
    bool handleDataMessage(char *buffer, ssize_t bytesRead);
    bool handleIntroMessage(char *buffer, ssize_t bytesRead);

    int server_fd;
    int epoll_fd;
};