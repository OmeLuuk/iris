#include "iris_chat.h"
#include "logging.h"

IrisChat::IrisChat(ClientConnectionManager &handler,
                   onMessageReceivedCallback onMessageReceived,
                   const std::string &username)
    : Engine(false, handler), onMessageReceived(onMessageReceived), username(username)
{
    SubscribeToTopic(username);
    SubscribeToTopic("General");
}

void IrisChat::onMessage(const MessageType type, int client_fd, const void *data, size_t size)
{
    const char *dataAsChars = static_cast<const char *>(data);
    log(LL::INFO, data, size);

    const uint8_t topicSize = static_cast<const uint8_t *>(data)[0];
    const std::string topic(dataAsChars + 1, dataAsChars + 1 + topicSize);

    const uint8_t senderSize = static_cast<const uint8_t *>(data)[topicSize + 1];
    const char *senderStart = dataAsChars + 1 + topicSize + 1;
    const std::string sender(senderStart, senderStart + senderSize);

    const char *msgStart = senderStart + senderSize;
    const std::string msg(msgStart, dataAsChars + size - msgStart);

    onMessageReceived(topic, sender, msg);
}

void IrisChat::EventCycle()
{
    handleMessages(0);
}

void IrisChat::SendChatMessage(const std::string &topic, const std::string &message)
{
    uint8_t topicSize = static_cast<uint8_t>(topic.size()); // Get size of topic

    // Use a vector to construct the message
    std::vector<char> msg;
    msg.push_back(topicSize);                                // append the topic size
    msg.insert(msg.end(), topic.begin(), topic.end());       // append the topic
    msg.push_back(static_cast<uint8_t>(username.size()));    // append username length
    msg.insert(msg.end(), username.begin(), username.end()); // append username
    msg.insert(msg.end(), message.begin(), message.end());   // append the input line

    // Send the message
    sendMessage(connectionManager.getFd(), MessageType::PUBLIC_MESSAGE, msg);
}

void IrisChat::SubscribeToTopic(const std::string &topic)
{
    sendMessage(connectionManager.getFd(), MessageType::SUBSCRIBE, topic);
}
