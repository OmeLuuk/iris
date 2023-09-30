#include "iris_client.h"

int main()
{
    IrisClient client(ClientType::BROADCAST_PRODUCER);

    const auto preparedMessage = client.createMsg(BROADCAST_PRODUCER, "127.0.0.1:1000");
    client.sendMsg(preparedMessage);

    return 0;
}