#include "iris_client.h"

#include "logging.h"

#include <iostream>
#include <netinet/tcp.h>
#include <cstring>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

IrisClient::IrisClient(ClientType type) : clientConnectionManager()
{
}

std::vector<char> IrisClient::createMsg(ClientType type, const char *msg)
{
    std::vector<char> fullMsg(1024, '\0');
    fullMsg[0] = static_cast<char>(type);
    size_t msgLength = std::min(strlen(msg), (size_t)1023);
    std::copy(msg, msg + msgLength, fullMsg.begin() + 1);
    return fullMsg;
}

void IrisClient::sendMsg(const std::vector<char> &msgToSend)
{
    clientConnectionManager.sendMessage(MessageType::INTRO, msgToSend);
}