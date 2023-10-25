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

    if (type == MessageType::PUBLIC_MESSAGE) 
        broadcastMessage(data, size);

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
    if (size > 255)
    {
        log(LL::ERROR, "Topic size exceeds 255 bytes. Ignoring subscription request.");
        return;
    }

    const auto topic = static_cast<const char *>(data);
    topicSubscribers[std::string(topic, size)].insert(client_fd);

    log(LL::DEBUG, "Subscribed % to topic %", std::to_string(client_fd), topic);
}

void IrisServer::broadcastMessage(const void *data, size_t size)
{
    if (size < 1)
    {
        log(LL::ERROR, "Received an empty message; ignoring");
        return;
    }

    uint8_t topicLength = *(static_cast<const uint8_t *>(data));

    if (topicLength >= size)
    {
        log(LL::ERROR, "Topic length cannot be longer than the message size");
        return;
    }

    const char *topicStart = static_cast<const char *>(data) + 1;
    std::string topic(topicStart, topicLength);

    auto subscribers = topicSubscribers.find(topic);
    if (subscribers != topicSubscribers.end())
    {
        // Send the remaining message to all subscribers.
        // const char *messageStart = topicStart + topicLength;
        // size_t messageSize = size - 1 - topicLength;

        for (int fd : subscribers->second)
        {
            sendMessage(fd, MessageType::PUBLIC_MESSAGE, data, size);
        }
    }
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