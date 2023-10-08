#include "iris_client.h"

int main()
{
    IrisClient client(ClientType::BROADCAST_PRODUCER);

    std::vector<char> fullMsg{'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'};
    client.sendMsg(MessageType::DATA, fullMsg);

    return 0;
}