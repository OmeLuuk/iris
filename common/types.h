#pragma once

#include "logging.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <variant>
#include <cstdint>

enum class UserStatus
{
    ONLINE = 0,
    OFFLINE = 1,
};

enum class ClientType
{
    UNKNOWN = 0,
    BROADCAST_PRODUCER = 1,
    BROADCAST_CONSUMER = 2,
    PROXY_CLIENT = 3,
    DNS_CLIENT = 4,
    IRIS_CHAT = 5,
};

class PublicMessage
{
public:
    PublicMessage(const std::string &sender, const std::string &topic, const std::string &content)
        : sender(sender), topic(topic), content(content) {}

    PublicMessage(const uint8_t *data, size_t size)
    {
        const char *dataAsChars = reinterpret_cast<const char *>(data);
        log(LL::INFO, data, size);

        const uint8_t topicSize = data[0];
        topic = std::string(dataAsChars + 1, dataAsChars + 1 + topicSize);

        const uint8_t senderSize = data[topicSize + 1];
        const char *senderStart = dataAsChars + 1 + topicSize + 1;
        sender = std::string(senderStart, senderStart + senderSize);

        const char *msgStart = senderStart + senderSize;
        content = std::string(msgStart, dataAsChars + size - msgStart);
    }

    std::vector<char> serialize() const
    {
        // Serialize the PublicMessage fields into a byte vector
        return {};
    }

    std::string sender;
    std::string topic;
    std::string content;
};

class UserUpdate
{
public:
    UserUpdate(const std::string &username, const UserStatus userStatus)
        : username(username), userStatus(userStatus) {}

    UserUpdate(const uint8_t *data, size_t size)
    {
        const unsigned char *charData = static_cast<const unsigned char *>(data);
        const ClientType type = static_cast<ClientType>(charData[0]);
        int offset = 1;
        const UserStatus status = static_cast<UserStatus>(charData[offset++]);
        const uint8_t length = static_cast<uint8_t>(charData[offset++]);
        const std::string username(charData + offset, charData + offset + length);
    }

    std::string username;
    UserStatus userStatus;
};

class ErrorMessage
{
};

class IntroMessage
{
};

enum MessageType
{
    ERROR = 0,
    INTRO = 1,
    DATA = 2,
    SUBSCRIBE = 3,
    PUBLIC_MESSAGE = 4,
    USER_UPDATE = 5, // [clienttype - 1 byte][UserStatus - 1 byte][username length L - 1 byte][username - L bytes]
};

using MessageVariant = std::variant<PublicMessage, ErrorMessage, IntroMessage>;

MessageVariant deserialize(const uint8_t *data, size_t size);

inline std::string ToString(ClientType clientType)
{
    switch (clientType)
    {
    case ClientType::BROADCAST_PRODUCER:
        return "BROADCAST_PRODUCER";
    case ClientType::BROADCAST_CONSUMER:
        return "BROADCAST_CONSUMER";
    case ClientType::PROXY_CLIENT:
        return "PROXY_CLIENT";
    case ClientType::DNS_CLIENT:
        return "DNS_CLIENT";
    case ClientType::IRIS_CHAT:
        return "IRIS_CHAT";
    }

    throw std::runtime_error("unknown clientType in ToString()");
}

struct ClientInfo
{
    ClientType clientType = ClientType::BROADCAST_CONSUMER;
    std::string address;
    int port = 0;
};