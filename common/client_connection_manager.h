#pragma once

#include "base_connection_manager.h"
#include <netinet/in.h>

class ClientConnectionManager : public BaseConnectionManager
{
public:
    ClientConnectionManager(const ClientType clientType);
    ~ClientConnectionManager();
    void onConnected(int server_fd);
    void onDisconnected(int server_fd);

private:
    void onMessageReceived(const int server_fd, const uint8_t *data, const size_t size) override;

    int createSocket();
    bool configureSocket(int sockfd);
    sockaddr_in prepareServerAddress();
    bool connectToServerAddress(int sockfd, const sockaddr_in &address);

    void setupConnection();
};