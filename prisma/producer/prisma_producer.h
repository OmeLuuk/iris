#pragma once

#include "client_connection_manager.h"
#include "engine.h"

#include <unistd.h>
#include <string>

class PrismaProducer : public Engine
{
public:
    PrismaProducer(ClientConnectionManager &handler);
    ~PrismaProducer();

private:
    void EventCycle() override;
    char count = 35;
};