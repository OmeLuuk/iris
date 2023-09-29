#include "iris_client.h"

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>  // For TCP_NODELAY
#include <cstring>
#include <arpa/inet.h>

bool IrisClient::connectToServer(const std::string& ipAddress, int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return false;
    }

    int flag = 1;
    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));  // Disable Nagle's algorithm

    sockaddr_in serverAddress; // this is the socket of the server we want to connect to
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr); // this function converts an IP address string (like "192.168.1.1") into a format suitable for the sockaddr_in structure

    if (connect(sockfd, (sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(sockfd);
        sockfd = -1;
        return false;
    }

    return true;
}

std::vector<char> IrisClient::createMsg(ClientType type, const char* msg)
{
    std::vector<char> fullMsg(1024, '\0'); 
    fullMsg[0] = static_cast<char>(type);
    size_t msgLength = std::min(strlen(msg), (size_t)1023);
    std::copy(msg, msg + msgLength, fullMsg.begin() + 1);
    return fullMsg;
}

bool IrisClient::sendMsg(const std::vector<char>& msgToSend)
{
    if (sockfd == -1) {
        std::cerr << "Not connected to any server" << std::endl;
        return false;
    }

    ssize_t bytesSent = send(sockfd, msgToSend.data(), msgToSend.size(), 0);
    if (bytesSent != static_cast<ssize_t>(msgToSend.size())) {
        std::cerr << "Failed to send the complete message" << std::endl;
        return false;
    }

    return true;
}



