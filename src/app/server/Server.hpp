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
#include <sys/stat.h>
#include <sys/types.h>

class Config;
class AllConfigs;

class Server{
public:
    Server();
    Server( std::map<int, Config*> );
    // int launchConfig();
    int launchServer();
    // std::vector<Config> configs_vector;
    std::map<int, Config*> configs_map;
    std::map<int, RequestParser> clientsReq;
    std::map<int, ResponseParser> clientsResp;
    
    std::vector<int> unique_ports;
    int getServersCountFromConf();
    int upServer(int s, int p);
    int setServSockets();
    void httpIO();
    int getCorrespondingLoc(RequestParser &);
public:
    int servers_count;
    std::vector<int> server_sockets;
    std::vector<int> client_sockets;
    int buff_size;
    int MAX_CLIENTS;
    int max_sd;
    int addrlen;
    struct sockaddr_in srv;
    fd_set readfds, writefds, tmpReadfds, tmpWritefds;

private:
    // int servers_count;
    // std::vector<int> server_sockets;
    // std::vector<Config> configs;
};

#endif