#include "types.h"
#include "iris_server.h"
#include "config.h"
#include "logging.h"

int main()
{
    ServerConnectionManager serverConnectionManager;
    IrisConfig::Config config("config.txt");
    int port = std::stoi(config.get("port"));

    IrisServer server(serverConnectionManager);
    server.ReactiveEventLoop();

    return 0;
}