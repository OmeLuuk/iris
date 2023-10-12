#include "iris_client.h"

#include "logging.h"

#include <iostream>
#include <netinet/tcp.h>
#include <cstring>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

IrisClient::IrisClient(ClientType type) : clientConnectionManager(type)
{
}