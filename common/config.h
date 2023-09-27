#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>

namespace IrisConfig {

class Config {
public:
    Config(const std::string &filePath);
    std::string get(const std::string &key) const;

private:
    std::map<std::string, std::string> configMap;
    void loadFromFile(const std::string &filePath);
};

}

#endif
