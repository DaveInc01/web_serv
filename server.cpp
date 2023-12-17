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
#include "./src/app/parser/RequestParser.hpp"
#define MAX_CLIENTS 10
#define SERVERCOUNT 2
struct sockaddr_in srv, client;
fd_set readfds, fw, fe, tmpReadfds;

struct {
    int ip;
    int port;
    int buff_size;
    std::string location;
    std::string root;
}   configs;

int main(){
    int ports[SERVERCOUNT] = {8880, 7770};
    int serverSockets[SERVERCOUNT];
    int activity, max_sd, sd, maxClients = MAX_CLIENTS;
    int nRet = 0;
    int addrlen, clientSockets[MAX_CLIENTS];
    char buff[400];
    
    for (int i = 0; i < SERVERCOUNT; i++)
    {

        serverSockets[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (serverSockets[i] < 0)
        {
            std::cout << "The socket not oppened" << std::endl;
        }
        else{
            std::cout << "socket has opened on port " << ports[i] << std::endl;
        }
        srv.sin_family = AF_INET;
        srv.sin_port = htons(ports[i]);
        srv.sin_addr.s_addr = INADDR_ANY;
        memset(&(srv.sin_zero), 0, 8);
        addrlen = sizeof(srv);
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
        for (int i = 0; i < SERVERCOUNT; i++)
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
        
        // new conection has come 
        for(int i = 0; i < SERVERCOUNT; i++)
        {
            if (FD_ISSET(serverSockets[i], &readfds) != 0)
            {
                int newSocket;
                if ((newSocket = accept(serverSockets[i], (struct sockaddr*)&srv, (socklen_t*)&addrlen)) < 0) {
                    perror("Accept failed");
                    exit(EXIT_FAILURE);
                }

                for (int i = 0; i < max_sd; i++)
                {
                    if(clientSockets[i] == 0)
                    {
                        clientSockets[i] = newSocket;
                        break;
                    }
                }
            }

            for(int i = 0; i < max_sd; i++)
            {
                sd = clientSockets[i];
                if(FD_ISSET(sd, &readfds))
                {
                    int valread = read(sd, buff, sizeof(buff));
                    if (valread == 0) {
                        // Connection closed by client
                        getpeername(sd, (struct sockaddr*)&srv, (socklen_t*)&addrlen);
                        std::cout << "Host disconnected, ip " << inet_ntoa(srv.sin_addr) << " , port " << ntohs(srv.sin_port) << std::endl << std::endl;
                        close(sd);
                        clientSockets[i] = 0;
                    } 
                    else{
                        char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
                        int send_res=send(sd,arr,sizeof(arr),0);
                        std::string strBuff(buff);
                        RequestParser request(strBuff, valread);
                        try{
                            request.launchParse();
                        }
                        catch(const char* error){
                            std::cout << error << std::endl;
                        }
                    }
                }
            }
        }
    
    }
    return 0;
}