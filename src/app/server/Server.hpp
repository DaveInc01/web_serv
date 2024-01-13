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
    int launchConfig();
    int launchServer();
    // std::vector<Config> configs_vector;
    std::map<int, Config*> configs_map;
    std::map<int, RequestParser> clientsReq;
    std::map<int, ResponseParser> clientsResp;

    int getServersCount();
    // static int upServer();
public:
    int MAX_CLIENTS;
    struct sockaddr_in srv;
    fd_set readfds, writefds, tmpReadfds, tmpWritefds;
private:
    // int servers_count;
    // std::vector<int> server_sockets;
    // std::vector<Config> configs;
};

#endif