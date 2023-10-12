#include "types.h"
#include "prisma_consumer.h"
#include "config.h"
#include "logging.h"

int main()
{
    ClientConnectionManager connectionManager(ClientType::BROADCAST_CONSUMER);
    PrismaConsumer consumer(connectionManager);
    consumer.ReactiveEventLoop();

    return 0;
}