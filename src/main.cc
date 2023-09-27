#include "../common/config.h"
#include <iostream>

int main() {
    IrisConfig::Config config("config.txt");
    int port = std::stoi(config.get("port"));

    return 0;
}