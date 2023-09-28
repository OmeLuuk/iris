#include <string>
#include <unistd.h>

class UltraLowLatencyClient {
public:
    UltraLowLatencyClient() : sockfd(-1) {}
    ~UltraLowLatencyClient() {
        if (sockfd != -1) {
            close(sockfd);
        }
    }
    bool connectToServer(const std::string& ipAddress, int port);
    bool sendMsg(const std::string& msg);

private:
    int sockfd;
};