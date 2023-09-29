#pragma once

#include "../server/types.h"
#include <string>
#include <vector>
#include <unistd.h>

class IrisClient {
public:
    IrisClient() : sockfd(-1) {}
    ~IrisClient() {
        if (sockfd != -1) {
            close(sockfd);
        }
    }
    bool connectToServer(const std::string& ipAddress, int port);
    std::vector<char> createMsg(ClientType type, const char* msg);
    bool sendMsg(const std::vector<char>& msg);

private:
    int sockfd;
};