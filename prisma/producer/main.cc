#include "types.h"
#include "prisma_producer.h"
#include "config.h"
#include "logging.h"

int main()
{
    ClientConnectionManager connectionManager(ClientType::BROADCAST_PRODUCER);
    PrismaProducer producer(connectionManager);
    producer.EventLoop(std::chrono::milliseconds(2000));

    return 0;
}