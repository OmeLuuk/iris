#pragma once

#include "types.h"
#include <cstring>

class EventHandler
{
public:
    virtual ~EventHandler() = default;
    virtual void onMessage(const MessageType messageType, const int fd, const void *data, size_t size){};
    virtual void onConnected(const int fd, const ClientType type){};
    virtual void onDisconnectedEvent(const int fd){};

    virtual void onNewMessage(const MessageVariant &message)
    {
        std::visit([this](const auto &msg)
                   { this->handleMessage(msg); },
                   message);
    }

protected:
    virtual void handleMessage(const PublicMessage &message)
    {
        log(LL::ERROR, "Unhandled PublicMessage.");
    }

    virtual void handleMessage(const ErrorMessage &message)
    {
        log(LL::ERROR, "Unhandled ErrorMessage.");
    }

    virtual void handleMessage(const IntroMessage &message)
    {
        log(LL::ERROR, "Unhandled IntroMessage.");
    }

    virtual void handleMessage(const SubscribeMessage &message)
    {
        log(LL::ERROR, "Unhandled SubscribeMessage.");
    }

    virtual void handleMessage(const UserUpdate &message)
    {
        log(LL::ERROR, "Unhandled UserUpdate.");
    }
};