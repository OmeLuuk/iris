#pragma once
#include <string>

enum ClientType
{
    BROADCAST_PRODUCER = 0,
    BROADCAST_CONSUMER = 1,
    PROXY_CLIENT = 2,
    DNS_CLIENT = 3,
};

struct ClientInfo
{
    ClientType clientType = ClientType::BROADCAST_CONSUMER;
    std::string address;
    int port = 0;
};