#include "iris_client.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>  // For TCP_NODELAY
#include <arpa/inet.h>

bool UltraLowLatencyClient::connectToServer(const std::string& ipAddress, int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    int flag = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));  // Disable Nagle's algorithm

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(sockfd);
        sockfd = -1;
        return false;
    }

    return true;
}

bool UltraLowLatencyClient::sendMsg(const std::string& msg)
{
    if (sockfd == -1) {
        std::cerr << "Not connected to any server" << std::endl;
        return false;
    }
        
    ssize_t bytesSent = send(sockfd, msg.c_str(), msg.size(), 0);
    if (bytesSent != static_cast<ssize_t>(msg.size())) {
        std::cerr << "Failed to send the complete message" << std::endl;
        return false;
    }
        
    return true;
}


