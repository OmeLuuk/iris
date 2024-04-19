#include "types.h"

MessageVariant deserialize(const uint8_t *data, size_t size)
{
    MessageType type = static_cast<MessageType>(data[0]);
    switch (type)
    {
    case MessageType::INTRO:
        return IntroMessage(data + 1, size - 1);
    case MessageType::PUBLIC_MESSAGE:
        return PublicMessage(data + 1, size - 1); // Construct and return a PublicMessage
    case MessageType::USER_UPDATE:
        return UserUpdate(data + 1, size - 1);
    case MessageType::ERROR:
        return ErrorMessage(/*...*/); // Construct and return an ErrorMessage
    case MessageType::SUBSCRIBE:
        return SubscribeMessage(data + 1, size - 1);
    case MessageType::DATA:
        break;
    }
}