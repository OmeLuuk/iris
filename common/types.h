#pragma once
#include <string>
#include <stdexcept>

enum MessageType
{
    ERROR = 0,
    INTRO = 1,
    DATA = 2
};

enum class ClientType
{
    UNKNOWN = 0,
    BROADCAST_PRODUCER = 1,
    BROADCAST_CONSUMER = 2,
    PROXY_CLIENT = 3,
    DNS_CLIENT = 4,
};

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
    }

    throw std::runtime_error("unknown clientType in ToString()");
}

struct ClientInfo
{
    ClientType clientType = ClientType::BROADCAST_CONSUMER;
    std::string address;
    int port = 0;
};