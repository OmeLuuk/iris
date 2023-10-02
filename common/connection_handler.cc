#include "connection_handler.h"
#include "logging.h"

ConnectionHandler::ConnectionHandler(MessageCallback onMessage, ConnectionCallback onConnect, ConnectionCallback onDisconnect)
    : onMessageCallback(std::move(onMessage)),
      onConnectedCallback(std::move(onConnect)),
      onDisconnectedCallback(std::move(onDisconnect))
{
}

// TODO: refactor to have less code in one function
void ConnectionHandler::onDataReceived(const int client_fd, const char *data, ssize_t bytesRead)
{
    if (bytesRead == 0)
    {
        disconnect(client_fd, "Client disconnected gracefully");
        return;
    }
    if (bytesRead == -1)
    {
        disconnect(client_fd, "Client closed connection with an error");
        return;
    }

    size_t processedBytes = 0;

    while (processedBytes < bytesRead)
    {
        Message &unfinishedMessage = clientBuffers[client_fd];

        size_t remainingBytes = bytesRead - processedBytes;

        if (!unfinishedMessage.size_known)
        {
            int bytesNeededForSize = 4 - unfinishedMessage.buffer.size();
            if (remainingBytes < bytesNeededForSize)
            {
                unfinishedMessage.buffer.insert(unfinishedMessage.buffer.end(), data + processedBytes, data + bytesRead);
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

        remainingBytes = bytesRead - processedBytes;

        ssize_t bytesNeeded = unfinishedMessage.total_message_size - unfinishedMessage.buffer.size();
        if (bytesNeeded <= remainingBytes)
        {
            unfinishedMessage.buffer.insert(unfinishedMessage.buffer.end(), data + processedBytes, data + processedBytes + bytesNeeded);
            onMessageReceived(client_fd, unfinishedMessage.buffer.data() + 4, unfinishedMessage.total_message_size - 4);

            unfinishedMessage.buffer.clear();
            unfinishedMessage.size_known = false;
            unfinishedMessage.total_message_size = 0;
            processedBytes += bytesNeeded;
        }
        else
        {
            unfinishedMessage.buffer.insert(unfinishedMessage.buffer.end(), data + processedBytes, data + bytesRead);
            processedBytes = bytesRead;
        }
    }
}

void ConnectionHandler::onMessageReceived(const int client_fd, const uint8_t *data, size_t size)
{

    // MessageType type = static_cast<MessageType>(buffer[0]);
    // switch (type)
    // {
    // case MessageType::ERROR:

    // case MessageType::INTRO:
    //     return handleIntroMessage(client_fd, buffer + 1, bytesRead - 1);
    // case MessageType::DATA:
    //     return handleDataMessage(client_fd, buffer + 1, bytesRead - 1);
    // default:
    //     log(LL::ERROR, "Unknown message type received!");
    // }
}

void ConnectionHandler::onConnected(int client_fd)
{
    // Implement logic.
}

void ConnectionHandler::onDisconnected(int client_fd)
{
    onDisconnectedCallback(client_fd);
}

void ConnectionHandler::disconnect(const int client_fd, const std::string &reason)
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
}