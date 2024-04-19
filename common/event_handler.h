#pragma once

#include "types.h"
#include <cstring>

class EventHandler
{
public:
    virtual ~EventHandler() = default;
    virtual void onConnected(const int fd, const ClientType type){};
    virtual void onDisconnectedEvent(const int fd){};

    virtual void onNewMessage(const MessageVariant &message, const int fd = -2)
    {
        std::visit([this, fd](const auto &msg)
                   { this->handleMessage(msg, fd); },
                   message);
    }

protected:
    virtual void handleMessage(const PublicMessage &message, const int fd)
    {
        log(LL::ERROR, "Unhandled PublicMessage.");
    }

    virtual void handleMessage(const ErrorMessage &message, const int fd)
    {
        log(LL::ERROR, "Unhandled ErrorMessage.");
    }

    virtual void handleMessage(const IntroMessage &message, const int fd)
    {
        log(LL::ERROR, "Unhandled IntroMessage.");
    }

    virtual void handleMessage(const SubscribeMessage &message, const int fd)
    {
        log(LL::ERROR, "Unhandled SubscribeMessage.");
    }

    virtual void handleMessage(const UserUpdate &message, const int fd)
    {
        log(LL::ERROR, "Unhandled UserUpdate.");
    }
};