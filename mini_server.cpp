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

#define PORT 8889

struct sockaddr_in srv;
fd_set fr, fw, fe;

int main(){

    int nRet = 0;

    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (nSocket < 0)
    {
        std::cout << "The socket not oppened" << std::endl;
    }
    else{
        std::cout << "socket has opened successfully\n";
    }
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_ANY;
    memset(&(srv.sin_zero), 0, 8);

    nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
    if(nRet < 0)
    {
        std::cout << "Fail to bind to loacal port\n";
        exit(EXIT_FAILURE);
    }
    else{
        std::cout << "The socket bined successfully\n";
    }

    nRet = listen(nSocket, 5);
    if(nRet < 0)
    {
        std::cout << "max count of connections was full(5)\n";
        exit(EXIT_FAILURE);
    }

    int n = 100;
    for (int nIndex = 0; nIndex < 64; nIndex++)
    {
        FD_SET(n, &fr);
        n++;
    }

    for (int nIndex = 0; nIndex < 64; nIndex++)
    {
        std::cout << fr.fds_bits[nIndex] << std::endl; 
    }

    return 0;
}