#include "iris_server.h"
#include "types.h"
#include "logging.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <vector>

IrisServer::IrisServer(ServerConnectionManager &handler)
    : Engine(true, handler)
{
}

IrisServer::~IrisServer()
{
}

void IrisServer::onMessage(const MessageType type, int client_fd, const void *data, size_t size)
{
    log(LL::DEBUG, "Received a message:");
    log(LL::DEBUG, data, size);

    if (type == MessageType::SUBSCRIBE)
        addSubscriber(client_fd, data, size);

    if (fdToClientType.at(client_fd) == ClientType::BROADCAST_PRODUCER)
    {
        for (int fd : broadcastConsumers)
        {
            sendMessage(fd, MessageType::DATA, data, size);
        }
    }
}

void IrisServer::addSubscriber(int client_fd, const void *data, size_t size)
{
    const auto topic = static_cast<const char *>(data);
    topicSubscribers[std::string(topic, size)].insert(client_fd);

    // log(LL::DEBUG, std::format("Subscribed {} to topic {}", client_fd, topic));
}

void IrisServer::onConnected(const int client_fd, const ClientType type)
{
    fdToClientType.insert({client_fd, type});

    if (type == ClientType::BROADCAST_CONSUMER)
        broadcastConsumers.insert(client_fd);
}

void IrisServer::onDisconnected(const int fd)
{
    if (fdToClientType.at(fd) == ClientType::BROADCAST_CONSUMER)
        broadcastConsumers.erase(fd);
    
    fdToClientType.erase(fd);
}