#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>
#include "config/Config.hpp"

class Server{
public:
    Server();
    static int launchServer();
private:
    int servers_count;
    std::vector<int> server_sockets;
    std::vector<Config> configs;
};

#endif