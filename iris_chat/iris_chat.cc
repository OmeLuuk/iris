#include "iris_chat.h"
#include "logging.h"

IrisChat::IrisChat(ClientConnectionManager &handler)
    : Engine(false, handler)
{
    const std::vector<char> msg {'T', 'E', 'S', 'T', ' ', 'T', 'O', 'P', 'I', 'C'};
    sendMessage(connectionManager.getFd(), MessageType::SUBSCRIBE, msg);
}

void IrisChat::onMessage(const MessageType type, int client_fd, const void *data, size_t size)
{
    log(LL::INFO, data, size);
}

bool isInputAvailable()
{
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);   // clear the set
    FD_SET(0, &set); // add our file descriptor 0 (stdin) to the set

    timeout.tv_sec = 0;
    timeout.tv_usec = 0; // set our timeout to 0 which makes select non-blocking

    return select(1, &set, NULL, NULL, &timeout) > 0; // check if data is available to read on stdin
}

void IrisChat::EventCycle()
{
    if (isInputAvailable())
    {
        std::string line;
        std::getline(std::cin, line); // get the line from the console

        std::string topic = "TEST TOPIC";
        uint8_t topicSize = static_cast<uint8_t>(topic.size()); // Get size of topic

        // Use a vector to construct the message
        std::vector<char> msg;
        msg.push_back(topicSize);                          // append the topic size
        msg.insert(msg.end(), topic.begin(), topic.end()); // append the topic
        msg.insert(msg.end(), line.begin(), line.end());   // append the input line

        // Send the message
        sendMessage(connectionManager.getFd(), MessageType::PUBLIC_MESSAGE, msg);
    }

    // other event cycle logic
}
