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

namespace
{
    std::string createUserUpdate(const UserStatus userStatus, const std::string &username)
    {
        std::string updateMessage = "";
        updateMessage += static_cast<char>(ClientType::IRIS_CHAT);
        updateMessage += static_cast<char>(userStatus);
        updateMessage += static_cast<char>(username.size());
        updateMessage += username;
        return updateMessage;
    }
}

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

    switch (type)
    {
    case MessageType::SUBSCRIBE:
        addSubscriber(client_fd, data, size);
        break;
    case MessageType::PUBLIC_MESSAGE:
        broadcastMessage(data, size);
        break;
    case MessageType::USER_UPDATE:
        handleUserUpdate(client_fd, data, size);
        break;
    }

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

    log(LL::DEBUG, "Subscribed % to topic %", std::to_string(client_fd), std::string(topic, size));
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
        for (int fd : subscribers->second)
        {
            sendMessage(fd, MessageType::PUBLIC_MESSAGE, data, size);
        }
    }
}

void IrisServer::handleUserUpdate(const int client_fd, const void *data, size_t size)
{
    const unsigned char *charData = static_cast<const unsigned char *>(data);
    const ClientType type = static_cast<ClientType>(charData[0]);
    int offset = 1;
    const UserStatus status = static_cast<UserStatus>(charData[offset++]);
    const uint8_t length = static_cast<uint8_t>(charData[offset++]);
    const std::string username(charData + offset, charData + offset + length);

    for (const auto [_, username] : fdToIrisChatClient)
    {
        const std::string update = createUserUpdate(UserStatus::ONLINE, username);
        sendMessage(client_fd, MessageType::USER_UPDATE, update);
    }

    fdToIrisChatClient.insert({client_fd, username});

    for (const int fd : clientTypeToFds[type])
        sendMessage(fd, MessageType::USER_UPDATE, data, size);
}

void IrisServer::onConnected(const int client_fd, const ClientType type)
{
    fdToClientType.insert({client_fd, type});
    clientTypeToFds[type].insert(client_fd);

    if (type == ClientType::BROADCAST_CONSUMER)
        broadcastConsumers.insert(client_fd);
}

void IrisServer::onDisconnected(const int fd)
{
    if (fdToClientType.at(fd) == ClientType::BROADCAST_CONSUMER)
        broadcastConsumers.erase(fd);

    clientTypeToFds[fdToClientType[fd]].erase(fd);
    fdToClientType.erase(fd);
    fdToIrisChatClient.erase(fd);
    for (auto [_, subscribers] : topicSubscribers)
        subscribers.erase(fd);
}