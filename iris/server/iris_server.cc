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

void IrisServer::handleMessage(const SubscribeMessage& message, const int fd)
{
    topicSubscribers[message.topic].insert(fd);
    log(LL::DEBUG, "Subscribed " + std::to_string(fd) + " to topic: " + message.topic);
}

void IrisServer::handleMessage(const PublicMessage& message, const int fd)
{
    auto subscribers = topicSubscribers.find(message.topic);
    if (subscribers != topicSubscribers.end())
    {
        for (int fd : subscribers->second)
        {
            uint8_t topicSize = static_cast<uint8_t>(message.topic.size()); // Get size of topic

            // Use a vector to construct the message
            std::vector<char> msg;
            msg.push_back(topicSize);                                // append the topic size
            msg.insert(msg.end(), message.topic.begin(), message.topic.end()); // append the topic
            msg.push_back(static_cast<uint8_t>(message.sender.size()));      // append username length
            msg.insert(msg.end(), message.sender.begin(), message.sender.end()); // append username
            msg.insert(msg.end(), message.content.begin(), message.content.end());   // append the input line

            sendMessage(fd, MessageType::PUBLIC_MESSAGE, msg);
        }
    }
}

void IrisServer::handleMessage(const UserUpdate &message, const int fd)
{
    for (const auto [_, username] : fdToIrisChatClient)
    {
        const std::string update = createUserUpdate(UserStatus::ONLINE, username);
        
        sendMessage(fd, MessageType::USER_UPDATE, update);
    }

    fdToIrisChatClient.insert({fd, message.username});

    for (const int fd : clientTypeToFds[message.type])
    {
        std::string updateMessage = "";
        updateMessage += static_cast<char>(message.type);
        updateMessage += static_cast<char>(message.userStatus);
        updateMessage += static_cast<char>(message.username.size());
        updateMessage += message.username;
        
        sendMessage(fd, MessageType::USER_UPDATE, updateMessage);
    }
}

void IrisServer::onConnected(const int client_fd, const ClientType type)
{
    fdToClientType.insert({client_fd, type});
    clientTypeToFds[type].insert(client_fd);

    log(LL::DEBUG, "A client connected with fd " + std::to_string(client_fd) + ".");

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