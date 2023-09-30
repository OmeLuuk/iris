#pragma once

#include "../server/types.h"
#include <string>
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

class IrisClient {
public:
    IrisClient(ClientType type);
    ~IrisClient();

    std::vector<char> createMsg(ClientType type, const char* msg);
    bool sendMsg(const std::vector<char>& msg);

private:
    int sockfd;
    ClientType clientType;

    int createSocket();
    bool configureSocket(int sockfd);
    sockaddr_in prepareServerAddress();
    bool connectToServerAddress(int sockfd, const sockaddr_in &address);

    void setupConnection();
};