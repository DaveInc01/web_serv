//Example code: A simple server side code, which echos back the received message. 
//Handle multiple socket connections with select and fd_set on Linux 
#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "RequestParser.hpp"
#include <fcntl.h>
#define MAX_CLIENTS 10
// #define servers_count 2
struct sockaddr_in srv;
fd_set readfds, writefds, fe, tmpReadfds;

struct config_t {
    int ip;
    int port;
    int buff_size;
    std::string location;
    std::string root;
};

int main(){
    std::vector<config_t> configs_v;
    config_t srv1_conf;
    srv1_conf.buff_size = 1024;
    srv1_conf.port = 8007;
    configs_v.push_back(srv1_conf);
    config_t srv2_conf;
    srv2_conf.buff_size = 1024;
    srv2_conf.port = 7007;
    configs_v.push_back(srv2_conf);
    char buff[20];

    std::map<int, RequestParser> clients;

    int servers_count = configs_v.size();
    int serverSockets[servers_count];
    int activity, max_sd, sd, maxClients = MAX_CLIENTS;
    int nRet = 0;
    int addrlen, clientSockets[MAX_CLIENTS];


    std::cout << "servers count is " << servers_count << std::endl;
    for (int i = 0; i < servers_count; i++)
    {

        serverSockets[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (serverSockets[i] < 0)
        {
            std::cout << "The socket not oppened" << std::endl;
        }
        else{
            std::cout << "socket has opened on port " << configs_v[i].port << std::endl;
        }
        srv.sin_family = AF_INET;
        srv.sin_port = htons(configs_v[i].port);
        srv.sin_addr.s_addr = INADDR_ANY;
        // inet_pton(AF_INET, "10.12.0.1", &srv.sin_addr);
        memset(&(srv.sin_zero), 0, 8);
        addrlen = sizeof(srv);
         int l = 1;
        setsockopt(serverSockets[i], SOL_SOCKET, SO_REUSEADDR, &l, sizeof(l));
        if (fcntl(serverSockets[i], F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
        {
            std::cout << "Fcntl error\n";
            exit(1);
        }
        nRet = bind(serverSockets[i], (sockaddr*)&srv, sizeof(sockaddr));
        if(nRet < 0)
        {
            std::cout << "Fail to bind to loacal port\n";
            exit(EXIT_FAILURE);
        }
        else{
            std::cout << "The socket bined successfully\n\n\n";
        }

        nRet = listen(serverSockets[i], 5);
        if(nRet < 0)
        {
            std::cout << "max count of connections was full(5)\n";
            exit(EXIT_FAILURE);
        }

        for(int i = 0; i < MAX_CLIENTS; i++){
            clientSockets[i] = 0;
        }
    }

    while (true)
    {
        FD_ZERO(&tmpReadfds);
        // FD_ZERO(&writefds);

        for (int i = 0; i < servers_count; i++)
        {
            FD_SET(serverSockets[i], &tmpReadfds);
            max_sd = serverSockets[i];
        }
        for(int i = 0; i < maxClients; i++){
            sd = clientSockets[i];
            if(sd > 0)
            {
                FD_SET(sd, &tmpReadfds);
            }
            if(sd > max_sd)
            {
                max_sd = sd;
            }
        }
        memcpy(&readfds, &tmpReadfds, sizeof(tmpReadfds));
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if((activity < 0) && (errno != EINTR))
        {
            perror("Select error");
        }
        for(int i = 0; i < servers_count; i++)
        {
            if (FD_ISSET(serverSockets[i], &readfds) != 0)
            {
                /* new conection has come */ 
                int newSocket;
                if ((newSocket = accept(serverSockets[i], (struct sockaddr*)&srv, (socklen_t*)&addrlen)) < 0) {
                    perror("Accept failed");
                    exit(EXIT_FAILURE);
                }

                for (int i = 0; i < MAX_CLIENTS; i++)
                {
                    if(clientSockets[i] == 0)
                    {
                        clientSockets[i] = newSocket;
                        std::pair<int, RequestParser> clients_elem;
                        clients_elem.first = newSocket;
                        clients_elem.second = RequestParser();
                        clients.insert(clients_elem);
                        break;
                    }
                }
            }

            for(int i = 0; i < max_sd; i++)
            {
                int valread;
                sd = clientSockets[i];
                if(FD_ISSET(sd, &readfds))
                {
                    valread = recv(sd, buff, sizeof(buff) - 1, 0);
                    buff[valread] = '\0';
                    std::cout << buff << std::endl;
                    if (valread > 0)
                    {
                        std::string strBuff(buff);
                        try{
                            clients.at(sd).launchParse(strBuff, strBuff.size());
                        }
                        catch(const char* error){
                            std::cout << error << std::endl;
                            break ;
                        }
                        if(clients.at(sd).getIsReqEnd())
                        {
                            char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
                            int send_res = send(sd,arr,sizeof(arr),0);
                            getpeername(sd, (struct sockaddr*)&srv, (socklen_t*)&addrlen);
                            std::cout << "Host disconnected, ip " << inet_ntoa(srv.sin_addr) << " , port " << ntohs(srv.sin_port) << std::endl << std::endl;
                            
                            clients.erase(sd); 
                            close(sd);
                            clientSockets[i] = 0;
                        }
                        else{
                        //    FD_SET(sd, &writefds); 
                        }
                        FD_CLR(sd, &readfds);
                    }
                }
                else if(FD_ISSET(sd, &writefds))
                {
                    std::cout << "IN FD_SET WRITE-\n";
                    valread = recv(sd, buff, sizeof(buff) - 1, 0);
                    buff[valread] = '\0';
                    if (valread > 0)
                    {
                        std::cout << "IN WRITE FD\n";
                        // printf("%s", buff);
                        std::string strBuff(buff);
                        try{
                            clients.at(sd).launchParse(strBuff, strBuff.size());
                        }
                        catch(const char* error){
                            std::cout << error << std::endl;
                            break ;
                        }
                        if(clients.at(sd).getIsReqEnd())
                        {
                            char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
                            int send_res = send(sd,arr,sizeof(arr),0);
                            getpeername(sd, (struct sockaddr*)&srv, (socklen_t*)&addrlen);
                            std::cout << "Host disconnected, ip " << inet_ntoa(srv.sin_addr) << " , port " << ntohs(srv.sin_port) << std::endl << std::endl;
                            
                            clients.erase(sd); 
                            close(sd);
                            clientSockets[i] = 0;
                            FD_CLR(sd, &writefds);
                        }
                    }
                }
            }
        }
    }
    return 0;
}


