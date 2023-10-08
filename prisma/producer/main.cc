#include "types.h"
#include "iris_client.h"
#include "config.h"
#include "logging.h"

int main()
{
    ClientConnectionManager connectionManager(ClientType::BROADCAST_PRODUCER);
    IrisClient irisClient(ClientType::BROADCAST_PRODUCER);

    return 0;
}