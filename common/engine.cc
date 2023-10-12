#include "engine.h"
#include "types.h"
#include "logging.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <thread>

namespace
{
    constexpr int MAX_EVENTS = 10;        // defines how many events we wish to process in one event loop
    constexpr int MAX_HANDLE_SIZE = 1024; // this is how much data from a message we will handle at once.
}

Engine::Engine(bool acceptConnections, BaseConnectionManager &connectionManager)
    : canAcceptConnections(acceptConnections), connectionManager(connectionManager)
{
    int connFd = connectionManager.getFd();
    epoll_fd = epoll_create1(0);                        // epoll is a Linux-specific mechanism that allows for such efficient monitoring. this creates an epoll instance, see it as a monitor, and gives us an id (fd) to find our monitor from inside of this app
    epoll_event event;                                  // this struct is used to define what kind of stuff we are interested in and want to monitor with our epoll
    event.events = EPOLLIN;                             // we are interested in input events (incoming connections/data), this is an event telling us there is data we recieved in our mailbox waiting for us to process
    event.data.fd = connFd;                             // the file descriptor of our server socket
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connFd, &event); // add the event to our events monitor, the epoll

    connectionManager.setHandler(this);
}

Engine::Engine(BaseConnectionManager &connectionManager)
    : connectionManager(connectionManager)
{
    connectionManager.setHandler(this);
}

Engine::~Engine()
{
    if (epoll_fd != -1)
    {
        close(epoll_fd);
    }
}

void Engine::ReactiveEventLoop()
{
    int acceptConnectionsFd = connectionManager.getFd();
    epoll_event events[MAX_EVENTS]; // allocates space for us to have epoll put events in so we can handle the events one by one in an event loop iteration
    while (true)
    {
        log(LL::INFO, "waiting for new messages...");

        int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // check the epoll monitor by its fd. if there iare one or more events in epoll's buffer ready to be processed, this method will put up to MAX_EVENTS of them into the events variable. otherwise, it will wait until the timeout before next lines of code are executed. the timeout of -1 means wait indefinitely
        for (int i = 0; i < num_fds; i++)
        {
            if (canAcceptConnections && events[i].data.fd == acceptConnectionsFd) // this means a client tries to connect to us because this fd only accepts incoming connections since it needs to get a connection request first since we use tcp
            {
                log(LL::INFO, "received a new client connection!");
                int client_fd = accept(acceptConnectionsFd, NULL, NULL); // this completes the tcp handshake and now we have accepted a connection with this client. the fd received from this represents the established connection and this fd is where we look for data we receive from the client that just connected to us
                epoll_event client_event;
                client_event.events = EPOLLIN;                                // we are interested in incoming data from this client
                client_event.data.fd = client_fd;                             // set the fd for finding which client sent us data later whenever the event triggers
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event); // add the event to our events monitor, the epoll
            }
            else if (events[i].events & EPOLLIN)
            {
                log(LL::DEBUG, "received data from a client!");
                char buffer[MAX_HANDLE_SIZE];                                            // this is some array where we will store
                ssize_t bytes_read = recv(events[i].data.fd, buffer, sizeof(buffer), 0); // read the data from this event using its fd and store it in buffer
                connectionManager.onDataReceived(events[i].data.fd, buffer, bytes_read); // handleMessage(events[i].data.fd, buffer, bytes_read);
            }
        }
    }
}

void Engine::EventLoop(const std::chrono::milliseconds waitTime = std::chrono::milliseconds(0))
{
    while (true)
    {
        EventCycle();

        if (waitTime > std::chrono::milliseconds(0)) [[unlikely]]
        {
            std::this_thread::sleep_for(waitTime);
        }
    }
}

void Engine::sendMessage(const MessageType type, const std::vector<char> &msgToSend)
{
    connectionManager.sendMessage(type, msgToSend);
}

void Engine::sendMessage(const MessageType type, const void *data, size_t size)
{
    connectionManager.sendMessage(type, data, size);
}

void Engine::EventCycle()
{
}

void Engine::Disconnect(int fd)
{
}

void Engine::onMessage(int fd, const void *data, size_t size) {}
void Engine::onConnected(const int fd, const ClientType type) {}

void Engine::onDisconnected(const int fd)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL); // we stop monitoring messages from this event bufer since it is disconnected
}