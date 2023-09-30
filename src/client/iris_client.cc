#include "iris_client.h"

#include <iostream>
#include <netinet/tcp.h>
#include <cstring>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

namespace
{
    const char *IRIS_IP = "127.0.0.1";
    const int IRIS_PORT = 8080;
    constexpr int MAX__RECONNECT_RETRIES = 10;
    constexpr int SLEEP_DURATION_MS = 100;
}

IrisClient::IrisClient(ClientType type) : clientType(type)
{
    std::cout << "Setting up Iris connection. Will throw if a connection cannot be established" << std::endl;
    setupConnection();
}

IrisClient::~IrisClient()
{
    if (sockfd != -1)
    {
        close(sockfd);
    }
}

int IrisClient::createSocket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
    }
    return sockfd;
}

bool IrisClient::configureSocket(int sockfd)
{
    int flag = 1;
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int)) < 0) // Disable Nagle's algorithm
    {
        std::cerr << "Error configuring socket" << std::endl;
        return false;
    }
    return true;
}

sockaddr_in IrisClient::prepareServerAddress()
{
    sockaddr_in serverAddress; // this is the socket of the server we want to connect to
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(IRIS_PORT);
    inet_pton(AF_INET, IRIS_IP, &serverAddress.sin_addr); // this function converts an IP address string (like "192.168.1.1") into a format suitable for the sockaddr_in structure
    return serverAddress;
}

bool IrisClient::connectToServerAddress(int sockfd, const sockaddr_in &address)
{
    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        std::cerr << "Error connecting to server" << std::endl;
        return false;
    }
    return true;
}

void IrisClient::setupConnection()
{
    for (int attempt = 0; attempt < MAX__RECONNECT_RETRIES; ++attempt)
    {
        sockfd = createSocket();
        if (sockfd == -1)
        {
            std::cerr << "Attempt [" << attempt + 1 << "] Error creating socket" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION_MS));
            continue; // skip the rest of the loop and try again
        }

        if (!configureSocket(sockfd))
        {
            std::cerr << "Attempt [" << attempt + 1 << "] Error configuring socket" << std::endl;
            close(sockfd);
            sockfd = -1;
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION_MS));
            continue;
        }

        sockaddr_in serverAddress = prepareServerAddress();
        if (!connectToServerAddress(sockfd, serverAddress))
        {
            std::cerr << "Attempt [" << attempt + 1 << "] Error connecting to server" << std::endl;
            close(sockfd);
            sockfd = -1;
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_DURATION_MS));
            continue;
        }

        std::cout << "Successfully established connection with Iris!" << std::endl;
        return;
    }

    throw std::runtime_error("Could not establish a connection with Iris");
}

std::vector<char> IrisClient::createMsg(ClientType type, const char *msg)
{
    std::vector<char> fullMsg(1024, '\0');
    fullMsg[0] = static_cast<char>(type);
    size_t msgLength = std::min(strlen(msg), (size_t)1023);
    std::copy(msg, msg + msgLength, fullMsg.begin() + 1);
    return fullMsg;
}

bool IrisClient::sendMsg(const std::vector<char> &msgToSend)
{
    if (sockfd == -1)
    {
        std::cerr << "Not connected to any server" << std::endl;
        return false;
    }

    ssize_t bytesSent = send(sockfd, msgToSend.data(), msgToSend.size(), 0);
    if (bytesSent != static_cast<ssize_t>(msgToSend.size()))
    {
        std::cerr << "Failed to send the complete message" << std::endl;
        return false;
    }

    return true;
}
