#include "server_connection_manager.h"
#include "logging.h"

#include <cstring>

ServerConnectionManager::ServerConnectionManager()
{
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
        handleDataMessage(client_fd, data + 1, size - 1);
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

    eventHandler->onConnected(client_fd);
}

void ServerConnectionManager::handleDataMessage(const int client_fd, const uint8_t *data, const size_t size)
{
    eventHandler->onMessage(client_fd, data, size);
}