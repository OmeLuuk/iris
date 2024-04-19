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

    sockaddr_in server_addr;                                                     // create an object that specifies where i will need to put the mailbox so other apps can send it to these specs in order for their messages to be received in the mailbox at this address
    server_addr.sin_family = AF_INET;                                            // define we will use IPv4
    server_addr.sin_port = htons(IRIS_PORT);                                     // we want to listen on this port
    server_addr.sin_addr.s_addr = INADDR_ANY;                                    // we are fine listening to messages from any source address (IPv4)
    if (bind(connectionFd, (sockaddr *)&server_addr, sizeof(server_addr)) == -1) // put the mailbox we created in the specified location. now if we look in our mailbox using our fd, we see what other apps sent to the address we specified earlier.
        log(LL::ERROR, "Could not bind socket to server_addr");
    if (listen(connectionFd, MAX_CONN_REQUESTS) == -1) // listen with a backlog of 10
        log(LL::ERROR, "Failed to listen to socket");
}

void ServerConnectionManager::onMessageReceived(const int client_fd, const uint8_t *data, const size_t size)
{
    MessageVariant message = deserialize(data, size);
    if (std::holds_alternative<IntroMessage>(message))
        handleIntroMessage(std::get<IntroMessage>(message), client_fd);
    else
        eventHandler->onNewMessage(message, client_fd);
}

void ServerConnectionManager::handleErrorMessage(const int client_fd, const uint8_t *data, const size_t size)
{
}

void ServerConnectionManager::handleIntroMessage(const IntroMessage& message, const int client_fd)
{
    log(LL::INFO, "ClientType: " + ToString(message.clientType));

    eventHandler->onConnected(client_fd, message.clientType);
}

void ServerConnectionManager::handleDataMessage(const MessageType type, const int client_fd, const uint8_t *data, const size_t size)
{
    eventHandler->onMessage(type, client_fd, data, size);
}