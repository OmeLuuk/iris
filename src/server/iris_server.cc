#include "iris_server.h"
#include "types.h"
#include "logging.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/epoll.h>
#include <unistd.h>
#include <cstring>

namespace
{
    int MAX_EVENTS = 10;        // defines how many events we wish to process in one event loop
    int MAX_CONN_REQUESTS = 10; // defines how many connection requests we will accept each time we emtpy our buffer. we can serve more clients, but they will need to try and connect again if there were 10 others trying to establish a connection with us at the same time
    int MAX_HANDLE_SIZE = 1024; // this is how much data from a message we will handle at once.
}

IrisServer::IrisServer(int port)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0); // tell OS, create a mailbox for me and give me an id (fd) so i can find it inside of my app here

    sockaddr_in server_addr;                                               // create an object that specifies where i will need to put the mailbox so other apps can send it to these specs in order for their messages to be received in the mailbox at this address
    server_addr.sin_family = AF_INET;                                      // define we will use IPv4
    server_addr.sin_port = htons(port);                                    // we want to listen on this port
    server_addr.sin_addr.s_addr = INADDR_ANY;                              // we are fine listening to messages from any source address (IPv4)
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)); // put the mailbox we created in the specified location. now if we look in our mailbox using our fd, we see what other apps sent to the address we specified earlier.
    listen(server_fd, MAX_CONN_REQUESTS);                                  // listen with a backlog of 10

    epoll_fd = epoll_create1(0);                                  // epoll is a Linux-specific mechanism that allows for such efficient monitoring. this creates an epoll instance, see it as a monitor, and gives us an id (fd) to find our monitor from inside of this app
    epoll_event server_event;                                     // this struct is used to define what kind of stuff we are interested in and want to monitor with our epoll
    server_event.events = EPOLLIN;                                // we are interested in input events (incoming connections/data), this is an event telling us there is data we recieved in our mailbox waiting for us to process
    server_event.data.fd = server_fd;                             // the file descriptor of our server socket
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &server_event); // add the event to our events monitor, the epoll
}

IrisServer::~IrisServer()
{
    if (server_fd != -1)
    {
        close(server_fd);
    }
    if (epoll_fd != -1)
    {
        close(epoll_fd);
    }
}

void IrisServer::spin()
{
    epoll_event events[MAX_EVENTS]; // allocates space for us to have epoll put events in so we can handle the events one by one in an event loop iteration
    while (true)
    {
        log(LL::INFO, "waiting for new messages...");

        int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // check the epoll monitor by its fd. if there iare one or more events in epoll's buffer ready to be processed, this method will put up to MAX_EVENTS of them into the events variable. otherwise, it will wait until the timeout before next lines of code are executed. the timeout of -1 means wait indefinitely
        for (int i = 0; i < num_fds; i++)
        {
            if (events[i].data.fd == server_fd) // this means a client tries to connect to us because this fd only accepts incoming connections since it needs to get a connection request first since we use tcp
            {
                log(LL::INFO, "received a new client connection!");
                int client_fd = accept(server_fd, NULL, NULL); // this completes the tcp handshake and now we have accepted a connection with this client. the fd received from this represents the established connection and this fd is where we look for data we receive from the client that just connected to us
                epoll_event client_event;
                client_event.events = EPOLLIN;                                // we are interested in incoming data from this client
                client_event.data.fd = client_fd;                             // set the fd for finding which client sent us data later whenever the event triggers
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event); // add the event to our events monitor, the epoll
            }
            else if (events[i].events & EPOLLIN)
            {
                log(LL::INFO, "received data from a client!");
                char buffer[MAX_HANDLE_SIZE];                                            // this is some array where we will store
                ssize_t bytes_read = recv(events[i].data.fd, buffer, sizeof(buffer), 0); // read the data from this event using its fd and store it in buffer
                if (bytes_read <= 0 ||                                                   // 0: client disconnected gracefully; 1: client closed connectin with an error
                    !handleMessage(buffer, bytes_read))                                  // message parsing was unsuccessful
                {
                    close(events[i].data.fd);                                    // we close the connection to this client
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL); // we stop monitoring messages from this event bufer since it is disconnected
                }
            }
        }
    }
}

bool IrisServer::handleMessage(char *buffer, ssize_t bytesRead)
{
    if (bytesRead < 1)
    {
        log(LL::ERROR, "Message was empty!");
        return false;
    }

    MessageType type = static_cast<MessageType>(buffer[0]);
    switch (type)
    {
    case MessageType::INTRO_MSG:
        return handleIntroMessage(buffer + 1, bytesRead - 1);
    case MessageType::DATA_MSG:
        return handleDataMessage(buffer + 1, bytesRead - 1);
    default:
        log(LL::ERROR, "Unknown message type received!");
        return false;
    }
}

bool IrisServer::handleIntroMessage(char *buffer, ssize_t bytesRead)
{
    if (bytesRead < 1)
    {
        log(LL::ERROR, "Intro message was empty!");
        return false; // Check if we at least have the ClientType
    }

    ClientType clientType = static_cast<ClientType>(buffer[0]);
    std::string ipAddress;
    uint16_t port = 0; // Initialize to prevent usage of uninitialized value

    // Find the separator in the buffer
    char *separatorPos = strchr(buffer + 1, ':');
    if (separatorPos)
    {
        ipAddress = std::string(buffer + 1, separatorPos - (buffer + 1));

        if (separatorPos + 1 + sizeof(uint16_t) <= buffer + bytesRead)
        {
            uint16_t *portPtr = reinterpret_cast<uint16_t *>(separatorPos + 1);
            port = ntohs(*portPtr); // Convert from network byte order
        }
        else
        {
            log(LL::ERROR, "Intro message did not have enough data to parse a port!");
            return false; // Not enough data for port
        }
    }
    else
    {
        log(LL::ERROR, "Intro message did not contain a :, meaning the IPAddress:Port format is not present!");
        return false; // Separator not found
    }

    log(LL::INFO, "ClientType: " + clientType);
    log(LL::INFO, "Address: " + ipAddress + ":" + std::to_string(port));

    return true;
}

bool IrisServer::handleDataMessage(char *buffer, ssize_t bytesRead)
{
    // Implementation for handling data messages

    // [Your implementation here]

    return true;
}