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

#define MAX_CLIENTS 10
#define PORT 8878

struct sockaddr_in srv, client;
fd_set readfds, fw, fe;

int main(){
    int activity, max_sd, sd, maxClients = MAX_CLIENTS;
    int nRet = 0;
    int addrlen, clientSockets[MAX_CLIENTS];
    char buff[1024];
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket < 0)
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
    addrlen = sizeof(srv);
    nRet = bind(serverSocket, (sockaddr*)&srv, sizeof(sockaddr));
    if(nRet < 0)
    {
        std::cout << "Fail to bind to loacal port\n";
        exit(EXIT_FAILURE);
    }
    else{
        std::cout << "The socket bined successfully\n";
    }

    nRet = listen(serverSocket, 5);
    if(nRet < 0)
    {
        std::cout << "max count of connections was full(5)\n";
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < MAX_CLIENTS; i++){
        clientSockets[i] = 0;
    }

    while (true)
    {
        FD_ZERO(&readfds);
        FD_SET(serverSocket, &readfds);

        max_sd = serverSocket;

        for(int i = 0; i < maxClients; i++){
            sd = clientSockets[i];
            if(sd > 0)
            {
                FD_SET(sd, &readfds);
            }
            if(sd > max_sd)
            {
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if((activity < 0) && (errno != EINTR))
        {
            perror("Select error");
        }
        // new conection has come 
        if (FD_ISSET(serverSocket, &readfds) != 0)
        {
            int newSocket;
            if ((newSocket = accept(serverSocket, (struct sockaddr*)&srv, (socklen_t*)&addrlen)) < 0) {
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
                    std::cout << "Host disconnected, ip " << inet_ntoa(srv.sin_addr) << " , port " << ntohs(srv.sin_port) << std::endl;
                    close(sd);
                    clientSockets[i] = 0;
                } 
                else{
                    char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
                    int send_res=send(sd,arr,sizeof(arr),0);
                    // send(sd, reply, strlen(reply), 0);
                    // printf("%s\n", buff);
                    // send(sd, buff, valread, 0);
                }
            }
        }

    }

    // int clientSocket = accept(serverSocket, (struct sockaddr*)&client, (socklen_t*)&addrlen);
    // if(clientSocket < 0){
    //     perror("accept: ");
    //     exit(EXIT_FAILURE);
    // }

    // connect(clientSocket, (struct sockaddr*)&srv, sizeof(srv));
    // send(clientSocket, message, strlen(message), 0);
    // recv(clientSocket, buff, sizeof(buff), 0);
    // printf("%s\n", buff);

    // close(serverSocket);
    // close(clientSocket);
    return 0;
}