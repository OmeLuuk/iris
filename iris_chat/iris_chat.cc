#include "iris_chat.h"
#include "logging.h"

IrisChat::IrisChat(ClientConnectionManager &handler, std::function<void(const std::string &)> onMessageReceived)
    : Engine(false, handler), onMessageReceived(onMessageReceived)
{
    const std::vector<char> msg{'T', 'E', 'S', 'T', ' ', 'T', 'O', 'P', 'I', 'C'};
    sendMessage(connectionManager.getFd(), MessageType::SUBSCRIBE, msg);
}

void IrisChat::onMessage(const MessageType type, int client_fd, const void *data, size_t size)
{
    log(LL::INFO, data, size);
    const char *charData = static_cast<const char *>(data);
    const std::string result(charData);
    onMessageReceived(result);
}

void IrisChat::EventCycle()
{
    handleMessages(0);
}

void IrisChat::SendChatMessage(const std::string &message)
{
    std::string topic = "TEST TOPIC";
    uint8_t topicSize = static_cast<uint8_t>(topic.size()); // Get size of topic

    // Use a vector to construct the message
    std::vector<char> msg;
    msg.push_back(topicSize);                              // append the topic size
    msg.insert(msg.end(), topic.begin(), topic.end());     // append the topic
    msg.insert(msg.end(), message.begin(), message.end()); // append the input line

    // Send the message
    sendMessage(connectionManager.getFd(), MessageType::PUBLIC_MESSAGE, msg);
}
