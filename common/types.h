#pragma once
#include <string>
#include <stdexcept>

enum MessageType
{
    ERROR = 0,
    INTRO = 1,
    DATA = 2
};

enum ClientType
{
    BROADCAST_PRODUCER = 0,
    BROADCAST_CONSUMER = 1,
    PROXY_CLIENT = 2,
    DNS_CLIENT = 3,
};

inline std::string ToString(ClientType clientType)
{
    switch (clientType)
    {
    case BROADCAST_PRODUCER:
        return "BROADCAST_PRODUCER";
    case BROADCAST_CONSUMER:
        return "BROADCAST_CONSUMER";
    case PROXY_CLIENT:
        return "PROXY_CLIENT";
    case DNS_CLIENT:
        return "DNS_CLIENT";
    }

    throw std::runtime_error("unknown clientType in ToString()");
}

struct ClientInfo
{
    ClientType clientType = ClientType::BROADCAST_CONSUMER;
    std::string address;
    int port = 0;
};