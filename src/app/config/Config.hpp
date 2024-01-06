#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config{
public:
    Config(){};
private:
    int ip;
    int port;
    int buff_size;
    std::string location;
};

#endif