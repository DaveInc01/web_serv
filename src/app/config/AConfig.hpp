#ifndef ACONFIG_HPP
#define ACONFIG_HPP

#include "server/Server.hpp"

class AConfig{
protected:
    std::vector<std::pair<int, std::string>> error_pages;
    int autoindex;
    int client_max_body_size;
    std::vector<std::string> methods;
    /* Type all directives bellow except "listen_ip" and "port"*/

};

class Location : AConfig{
public:
    Location(){};
};

class Config : public AConfig{
public:
    Config(){};
    std::map<std::string, Location> locations;
    std::string listen_ip;
    int port;
};


#endif