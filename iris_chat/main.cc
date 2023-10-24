#include "iris_chat.h"

#include "client_connection_manager.h"

int main()
{
    ClientConnectionManager connectionManager(ClientType::IRIS_CHAT);
    IrisChat irisChat(connectionManager);
    irisChat.EventLoop(std::chrono::milliseconds(10));
    return 0;
}