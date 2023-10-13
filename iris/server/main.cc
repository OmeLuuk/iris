#include "types.h"
#include "iris_server.h"
#include "config.h"
#include "logging.h"

int main()
{
    ServerConnectionManager serverConnectionManager;
    IrisServer server(serverConnectionManager);
    server.ReactiveEventLoop();

    return 0;
}