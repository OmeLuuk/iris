#include "config.h"
#include <fstream>

namespace IrisConfig {

Config::Config(const std::string &filePath) {
    loadFromFile(filePath);
}

void Config::loadFromFile(const std::string &filePath) {
    std::ifstream configFile(filePath);
    std::string line;

    while (getline(configFile, line)) {
        size_t delimiterPos = line.find('=');
        if (delimiterPos != std::string::npos) {
            std::string key = line.substr(0, delimiterPos);
            std::string value = line.substr(delimiterPos + 1);
            configMap[key] = value;
        }
    }
}

std::string Config::get(const std::string &key) const {
    if (configMap.find(key) != configMap.end()) {
        return configMap.at(key);
    }
    throw std::runtime_error("Error loading config, key " + key + " did not exist in config.txt!");
}

}
