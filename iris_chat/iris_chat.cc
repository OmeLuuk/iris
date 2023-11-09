#include "iris_chat.h"
#include "logging.h"

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

IrisChat::IrisChat(ClientConnectionManager &handler,
                   OnMessageReceivedCallback onMessageReceived,
                   const std::string &username,
                   OnUserStatusUpdateCallback onUserStatusChanged)
    : Engine(false, handler), onMessageReceived(onMessageReceived), username(username), onUserStatusChanged(onUserStatusChanged)
{
    SubscribeToTopic(username);
    SubscribeToTopic("General");
    SubscribeToTopic("Other");
    std::string onlineStatusMessage = createUserUpdate(UserStatus::ONLINE, username);
    sendMessage(connectionManager.getFd(), MessageType::USER_UPDATE, onlineStatusMessage);
}

void IrisChat::onMessage(const MessageType type, int client_fd, const void *data, size_t size)
{
    if (type == MessageType::PUBLIC_MESSAGE)
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
    else if (type == MessageType::USER_UPDATE)
    {
        const unsigned char *charData = static_cast<const unsigned char *>(data);
        const ClientType type = static_cast<ClientType>(charData[0]);
        int offset = 1;
        const UserStatus status = static_cast<UserStatus>(charData[offset++]);
        const uint8_t length = static_cast<uint8_t>(charData[offset++]);
        const std::string username(charData + offset, charData + offset + length);
        onUserStatusChanged(username, status);
    }
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
// todo : subscribe upon opening a tab, receive updates of online person including snapshots from iris, fix receiving own messages and just paste them locally
