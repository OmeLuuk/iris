#include "../common/config.h"
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/epoll.h>

namespace
{
    int MAX_EVENTS = 10; // defines how many events we wish to process in one event loop
}

int main() {
    IrisConfig::Config config("config.txt");
    int port = std::stoi(config.get("port"));

    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // tell OS, create a mailbox for me and give me an id (fd) so i can find it inside of my app here

    sockaddr_in server_addr; // create an object that specifies where i will need to put the mailbox so other apps can send it to these specs in order for their messages to be received in the mailbox at this address
    server_addr.sin_family = AF_INET; // define we will use IPv4
    server_addr.sin_port = htons(port); // we want to listen on this port
    server_addr.sin_addr.s_addr = INADDR_ANY; // we are fine listening to messages from any source address (IPv4)
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)); // put the mailbox we created in the specified location. now if we look in our mailbox using our fd, we see what other apps sent to the address we specified earlier.



    int epoll_fd = epoll_create1(0); // epoll is a Linux-specific mechanism that allows for such efficient monitoring. this creates an epoll instance, see it as a monitor, and gives us an id (fd) to find our monitor from inside of this app
    epoll_event ev; // this struct is used to define what kind of stuff we are interested in and want to monitor with our epoll
    ev.events = EPOLLIN; // we are interested in input events (incoming connections/data), this is an event telling us there is data we recieved in our mailbox waiting for us to process
    ev.data.fd = server_fd; // the file descriptor of our server socket
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev); // add the event to our events monitor, the epoll








    epoll_event events[MAX_EVENTS]; // allocates space for us to have epoll put events in so we can handle the events one by one in an event loop iteration
    while (true) {
        int num_fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // check the epoll monitor by its fd. if there iare one or more events in epoll's buffer ready to be processed, this method will put up to MAX_EVENTS of them into the events variable. otherwise, it will wait until the timeout before next lines of code are executed. the timeout of -1 means wait indefinitely
        for (int i = 0; i < num_fds; i++) {
            if (events[i].data.fd == server_fd) {
                // Accept new connection
                int client_fd = accept(server_fd, NULL, NULL);
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
            } else if (events[i].events & EPOLLIN) {
                // Handle data from client
                char buffer[1024];
                ssize_t bytes_read = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (bytes_read <= 0) {
                    // Client disconnected
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                } else {
                    // Process the message, for simplicity we'll just print it
                    std::cout << "Received message: " << std::string(buffer, bytes_read) << std::endl;
                }
            }
        }
    }






    return 0;
}