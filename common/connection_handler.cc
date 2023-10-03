#include "connection_handler.h"
#include "logging.h"

#include <cstring>

ConnectionHandler::ConnectionHandler(MessageCallback onMessage, ConnectionCallback onConnect, ConnectionCallback onDisconnect)
    : onMessageCallback(std::move(onMessage)),
      onConnectedCallback(std::move(onConnect)),
      onDisconnectedCallback(std::move(onDisconnect))
{
}

void ConnectionHandler::onMessageReceived(const int client_fd, const uint8_t *data, const size_t size)
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
        handleDataMessage(client_fd, data + 1, size - 1);
        break;
    default:
        log(LL::ERROR, "Unknown message type received!");
    }
}

void ConnectionHandler::handleErrorMessage(const int client_fd, const uint8_t *data, const size_t size)
{
    
}

void ConnectionHandler::handleIntroMessage(const int client_fd, const uint8_t *data, const size_t size)
{
    ClientType clientType = static_cast<ClientType>(data[0]);

    log(LL::INFO, "ClientType: " + ToString(clientType));
}

void ConnectionHandler::handleDataMessage(const int client_fd, const uint8_t *data, const size_t size)
{
}

void ConnectionHandler::onConnected(int client_fd)
{
    // Implement logic.
}

void ConnectionHandler::onDisconnected(int client_fd)
{
    onDisconnectedCallback(client_fd);
}