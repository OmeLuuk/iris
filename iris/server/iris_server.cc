#include "iris_server.h"
#include "types.h"
#include "logging.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <vector>

IrisServer::IrisServer(int port, ServerConnectionManager &handler)
    : Engine(port, handler)
{
}

IrisServer::~IrisServer()
{
}

void IrisServer::onMessage(int client_fd, const void *data, size_t size)
{
    log(LL::INFO, "Received a message:");
    log(LL::INFO, data, size);
}

void IrisServer::onConnected(const int client_fd)
{
}