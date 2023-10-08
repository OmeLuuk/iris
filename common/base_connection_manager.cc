#include "base_connection_manager.h"
#include "logging.h"

#include <string>
#include <functional>
#include <cstddef>
#include <cstdint>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

void BaseConnectionManager::sendMessage(MessageType type, const std::vector<char> &message)
{
    if (sockfd == -1)
    {
        log(LL::ERROR, "Not connected to any server");
        return;
    }

    uint32_t msgSizeNetworkOrder = htonl(static_cast<uint32_t>(message.size() + 5)); // +1 for the message type byte, +4 for the message size
    std::vector<char> fullMsg(sizeof(msgSizeNetworkOrder) + message.size() + 1);     // +1 for message type

    // Copy the size, message type, and message into the full message
    std::memcpy(fullMsg.data(), &msgSizeNetworkOrder, sizeof(msgSizeNetworkOrder));
    fullMsg[sizeof(msgSizeNetworkOrder)] = static_cast<char>(MessageType::INTRO);
    std::copy(message.begin(), message.end(), fullMsg.begin() + sizeof(msgSizeNetworkOrder) + 1); // +1 for the message type byte

    if constexpr (enableDebugLogging)
    {
        std::string s;
        for (int i = 0; i < fullMsg.size(); i++)
            s += std::to_string(static_cast<int>(fullMsg[i])) + ",";
        log(LL::DEBUG, s);
    }

    ssize_t bytesSent = send(sockfd, fullMsg.data(), fullMsg.size(), 0);
    if (bytesSent != static_cast<ssize_t>(fullMsg.size()))
    {
        log(LL::ERROR, "Failed to send the complete message");
    }
}

// TODO: refactor to have less code in one function
void BaseConnectionManager::onDataReceived(const int fd, const char *data, ssize_t bytes_read)
{
    if (bytes_read == 0)
    {
        disconnect(fd, "client disconnected gracefully");
        return;
    }
    if (bytes_read == -1)
    {
        disconnect(fd, "client closed connection with an error");
        return;
    }

    if (!eventHandler)
    {
        log(LL::ERROR, "received data while there is no handler to handle it");
        return;
    }

    size_t processedBytes = 0;

    while (processedBytes < bytes_read)
    {
        Message &unfinishedMessage = clientBuffers[fd];

        size_t remainingBytes = bytes_read - processedBytes;

        if (!unfinishedMessage.size_known)
        {
            int bytesNeededForSize = 4 - unfinishedMessage.buffer.size();
            if (remainingBytes < bytesNeededForSize)
            {
                unfinishedMessage.buffer.insert(unfinishedMessage.buffer.end(), data + processedBytes, data + bytes_read);
                return;
            }
            else
            {
                unfinishedMessage.buffer.insert(unfinishedMessage.buffer.end(), data + processedBytes, data + processedBytes + bytesNeededForSize);
                unfinishedMessage.size_known = true;
                unfinishedMessage.total_message_size = ntohl(*reinterpret_cast<const int *>(unfinishedMessage.buffer.data()));
                processedBytes += bytesNeededForSize;
            }
        }

        remainingBytes = bytes_read - processedBytes;

        ssize_t bytesNeeded = unfinishedMessage.total_message_size - unfinishedMessage.buffer.size();
        if (bytesNeeded <= remainingBytes)
        {
            unfinishedMessage.buffer.insert(unfinishedMessage.buffer.end(), data + processedBytes, data + processedBytes + bytesNeeded);
            onMessageReceived(fd, unfinishedMessage.buffer.data() + 4, unfinishedMessage.total_message_size - 4);

            unfinishedMessage.buffer.clear();
            unfinishedMessage.size_known = false;
            unfinishedMessage.total_message_size = 0;
            processedBytes += bytesNeeded;
        }
        else
        {
            unfinishedMessage.buffer.insert(unfinishedMessage.buffer.end(), data + processedBytes, data + bytes_read);
            processedBytes = bytes_read;
        }
    }
}

void BaseConnectionManager::setHandler(EventHandler *newEventHandler)
{
    if (eventHandler)
        log(LL::ERROR, "eventhandler set attempt while there is another eventhandler already");
    else
        eventHandler = newEventHandler;
}

void BaseConnectionManager::resetHandler(EventHandler *callingEventHandler)
{
    if (callingEventHandler == eventHandler)
        eventHandler = nullptr;
    else
        log(LL::ERROR, "eventhandler reset attempt by non-current eventhandler");
}

void BaseConnectionManager::disconnect(const int client_fd, const std::string &reason)
{
    std::vector<char> message;
    message.push_back(static_cast<char>(MessageType::ERROR));
    message.insert(message.end(), reason.begin(), reason.end());
    ssize_t bytesSent = send(client_fd, message.data(), message.size(), 0);

    if (bytesSent < 0)
    {
        log(LL::ERROR, "Failed to send error message to client before disconnecting.");
    }

    close(client_fd); // we close the connection to this client
    clientBuffers.erase(client_fd);
    log(LL::INFO, "Client disconnected with reason: " + reason);

    eventHandler->onDisconnected(client_fd);
}