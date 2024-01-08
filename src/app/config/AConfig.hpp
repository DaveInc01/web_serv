#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class AConfig{
public:
    AConfig(){};
protected:
    int ip;
    int port;
    int buff_size;
    std::string location;
};

#endif