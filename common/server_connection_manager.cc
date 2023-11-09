#include "server_connection_manager.h"
#include "logging.h"

#include <cstring>

namespace
{
    const int IRIS_PORT = 8080;
    constexpr int MAX_CONN_REQUESTS = 10; // defines how many connection requests we will accept each time we emtpy our buffer. we can serve more clients, but they will need to try and connect again if there were 10 others trying to establish a connection with us at the same time
}

ServerConnectionManager::ServerConnectionManager()
{
    connectionFd = socket(AF_INET, SOCK_STREAM, 0); // tell OS, create a mailbox for me and give me an id (fd) so i can find it inside of my app here

    if (connectionFd == -1)
        log(LL::ERROR, "Could not create socket");

    sockaddr_in server_addr;                                           // create an object that specifies where i will need to put the mailbox so other apps can send it to these specs in order for their messages to be received in the mailbox at this address
    server_addr.sin_family = AF_INET;                                  // define we will use IPv4
    server_addr.sin_port = htons(IRIS_PORT);                                // we want to listen on this port
    server_addr.sin_addr.s_addr = INADDR_ANY;                          // we are fine listening to messages from any source address (IPv4)
    if (bind(connectionFd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1) // put the mailbox we created in the specified location. now if we look in our mailbox using our fd, we see what other apps sent to the address we specified earlier.
        log(LL::ERROR, "Could not bind socket to server_addr");
    if (listen(connectionFd, MAX_CONN_REQUESTS) == -1)                           // listen with a backlog of 10
        log(LL::ERROR, "Failed to listen to socket");
}

void ServerConnectionManager::onMessageReceived(const int client_fd, const uint8_t *data, const size_t size)
{
    MessageType type = static_cast<MessageType>(data[0]);
    switch (type)
    {
    case MessageType::ERROR:
        handleErrorMessage(client_fd, data + 1, size - 1);
        break;
    case MessageType::INTRO:
        handleIntroMessage(client_fd, data + 1, size - 1);
        break;
    case MessageType::DATA:
    case MessageType::PUBLIC_MESSAGE:
    case MessageType::SUBSCRIBE:
    case MessageType::USER_UPDATE:
        handleDataMessage(type, client_fd, data + 1, size - 1);
        break;
    default:
        log(LL::ERROR, "Unknown message type received!");
    }
}

void ServerConnectionManager::handleErrorMessage(const int client_fd, const uint8_t *data, const size_t size)
{
}

void ServerConnectionManager::handleIntroMessage(const int client_fd, const uint8_t *data, const size_t size)
{
    ClientType clientType = static_cast<ClientType>(data[0]);

    log(LL::INFO, "ClientType: " + ToString(clientType));

    eventHandler->onConnected(client_fd, clientType);
}

void ServerConnectionManager::handleDataMessage(const MessageType type, const int client_fd, const uint8_t *data, const size_t size)
{
    eventHandler->onMessage(type, client_fd, data, size);
}