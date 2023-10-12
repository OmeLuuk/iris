#include "prisma_producer.h"
#include "types.h"
#include "logging.h"

PrismaProducer::PrismaProducer(ClientConnectionManager &handler)
    : Engine(handler)
{
}

PrismaProducer::~PrismaProducer()
{
}

void PrismaProducer::EventCycle()
{
    const std::vector<char> msg = {'m', 'e', 's', 's', 'a', 'g', 'e', count++};
    sendMessage(MessageType::DATA, msg);
}