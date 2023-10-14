#include "prisma_consumer.h"
#include "types.h"
#include "logging.h"

PrismaConsumer::PrismaConsumer(ClientConnectionManager &handler)
    : Engine(false, handler)
{
}

PrismaConsumer::~PrismaConsumer()
{
}

void PrismaConsumer::onMessage(const MessageType type, int client_fd, const void *data, size_t size)
{
    log(LL::INFO, data, size);
}