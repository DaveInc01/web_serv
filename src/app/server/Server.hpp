#ifndef SERVER_HPP
#define SERVER_HPP

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> 
#include <arpa/inet.h>  
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fcntl.h>
#include "request/RequestParser.hpp"
#include "response/ResponseParser.hpp"
#include "config/AllConfigs.hpp"

class Config;

class Server{
public:
    Server();
    static int launchConfig();
    static int launchServer();
    // std::vector<Config> configs_vector;
    static std::map<int, Config*> configs_map;
    static std::map<int, RequestParser> clientsReq;
    static std::map<int, ResponseParser> clientsResp;

    static int getServersCount();
    // static int upServer();
private:
    int servers_count;
    // std::vector<int> server_sockets;
    // std::vector<Config> configs;
};

#endif