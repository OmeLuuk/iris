#pragma once

#include "client_connection_manager.h"
#include "types.h"
#include <string>
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

class IrisClient {
public:
    IrisClient(ClientType type);

    std::vector<char> createMsg(ClientType type, const char* msg);
    void sendMsg(const MessageType type, const std::vector<char>& msg);

private:
    ClientConnectionManager clientConnectionManager;
};