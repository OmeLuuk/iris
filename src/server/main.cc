#include "types.h"
#include "iris_server.h"
#include "config.h"
#include "logging.h"

namespace
{
    int MAX_EVENTS = 10;        // defines how many events we wish to process in one event loop
    int MAX_CONN_REQUESTS = 10; // defines how many connection requests we will accept each time we emtpy our buffer. we can serve more clients, but they will need to try and connect again if there were 10 others trying to establish a connection with us at the same time
    int MAX_HANDLE_SIZE = 1024; // this is how much data from a message we will handle at once.
}

int main()
{
    IrisConfig::Config config("config.txt");
    int port = std::stoi(config.get("port"));

    IrisServer server(port);
    server.spin();

    return 0;
}