#include "iris_client.h"

int main() {
    UltraLowLatencyClient client;

    if (client.connectToServer("127.0.0.1", 8080)) {
        client.sendMsg("Hello, Server!");
    }

    return 0;
}