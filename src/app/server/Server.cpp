#include "Server.hpp"

Server::Server(){
    this->MAX_CLIENTS = 5;
}

struct config_t {
    char* host;
    int port;
    int buff_size;
    std::string location;
    std::string root;
};

int Server::launchConfig(){
    AllConfigs k;
    k.readConff();
    // k.print_AllServs();
    Server::configs_map = k._AllServs;
    return 0;
}

int Server::launchServer()
{
    std::vector<config_t> configs_v;
    config_t srv1_conf;
    srv1_conf.buff_size = 1024;
    srv1_conf.host = (char *)"127.0.0.1";
    srv1_conf.port = 8008;
    configs_v.push_back(srv1_conf);
    config_t srv2_conf;
    srv2_conf.host = (char *)"127.0.0.2";
    srv2_conf.buff_size = 1024;
    srv2_conf.port = 7008;
    configs_v.push_back(srv2_conf);
    char buff[200];
    int servers_count = configs_v.size();
    int serverSockets[servers_count];
    int activity, max_sd, sd;
    int nRet = 0;
    int addrlen, clientSockets[this->MAX_CLIENTS];

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
        std::cout << configs_v[i].port << std::endl;
        srv.sin_port = htons(configs_v[i].port);
        inet_pton(AF_INET, "0.0.0.0", &srv.sin_addr);
        // inet_pton(AF_INET, "127.0.0.1", &srv.sin_addr);
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

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&tmpReadfds);
    FD_ZERO(&tmpWritefds);

    // FD_ZERO(&writefds);

    for (int i = 0; i < servers_count; i++)
    {
        FD_SET(serverSockets[i], &readfds);
        // if(serverSockets[i] > max_sd)
            max_sd = serverSockets[i];
    }

    for(int i = 0; i < MAX_CLIENTS; i++){
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

    int newSocket = 0;
    while (true)
    {
        tmpReadfds = readfds;
        tmpWritefds = writefds;
        do {
            activity = select(max_sd + 1, &tmpReadfds, &tmpWritefds, NULL, NULL);
        } while (activity == -1);
        // activity = select(max_sd + 1, &tmpReadfds, &tmpWritefds, NULL, NULL);
        if((activity < 0) && (errno != EINTR))
        {
            std::cout << "activity - " << activity << std::endl;
            perror("Select error");
        }
        for(int i = 0; i < servers_count; i++)
        {
            if (FD_ISSET(serverSockets[i], &tmpReadfds) != 0)
            {
                /* new conection has come */ 
                if ((newSocket = accept(serverSockets[i], (struct sockaddr*)&srv, (socklen_t*)&addrlen)) < 0) {
                    std::cout << "Socket is returning - " << newSocket <<std::endl;
                    perror("Accept failed");
                    exit(EXIT_FAILURE);
                }
                else{
                    std::cout << "Accept the fd number - " << newSocket << std::endl;
                    
                }
            }
        }
        if(FD_ISSET(newSocket, &readfds) == 0)
        {
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if(clientSockets[i] == 0)
                {
                    FD_SET(newSocket, &readfds);
                    fcntl(newSocket, F_SETFL, O_NONBLOCK);
                    clientSockets[i] = newSocket;
                    std::cout << newSocket << std::endl;
                    std::pair<int, RequestParser> clients_req_elem;
                    clients_req_elem.first = newSocket;
                    clients_req_elem.second = RequestParser();
                    clientsReq.insert(clients_req_elem);
                    if(newSocket > max_sd)
                    {
                        max_sd = newSocket;
                    }
                    break;
                }
            }
        }

        for(int i = 0; i < MAX_CLIENTS; i++)
        {
            int valread;
            sd = clientSockets[i];
            if(FD_ISSET(sd, &tmpReadfds))
            {
                valread = recv(sd, buff, sizeof(buff) - 1, 0);
                buff[valread] = '\0';

                // std::cout <<"chunk - " << buff << std::endl;

                if (valread > 0)
                {
                    std::string strBuff(buff);
                    try{
                        clientsReq.at(sd).launchParse(strBuff, strBuff.size());
                    }
                    catch(const char* error){
                        std::cout << error << std::endl;
                        break ;
                    }
                    if(clientsReq.at(sd).getIsReqEnd())
                    {
                        FD_SET(sd, &writefds);
                        FD_CLR(sd, &readfds);
                        FD_CLR(sd, &tmpReadfds);

                        std::pair<int, ResponseParser> clients_resp_elem;
                        clients_resp_elem.first = sd;
                        clients_resp_elem.second = ResponseParser(clientsReq.at(sd));
                        clientsResp.insert(clients_resp_elem);
                        
                    }
                    FD_ZERO(&tmpReadfds);
                    FD_ZERO(&tmpWritefds);
                    continue;
                }
                else if(valread == 0){
                    /* 400 code */
                }
            }
            else if(FD_ISSET(sd, &tmpWritefds))
            {
                // if(clientsResp.at(sd).is_finsh)
                char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
                int send_res = send(sd,arr,sizeof(arr),0);
                std::cout << clientsReq.at(sd).getHttpReq();
                getpeername(sd, (struct sockaddr*)&srv, (socklen_t*)&addrlen);
                std::cout << "Host disconnected, ip " << inet_ntoa(srv.sin_addr) << " , port " << ntohs(srv.sin_port) << std::endl << std::endl;
                
                clientsReq.erase(sd); 
                close(sd);

                FD_CLR(sd, &writefds);
                FD_CLR(sd, &tmpWritefds);
                FD_ZERO(&tmpWritefds);
                FD_ZERO(&tmpReadfds);
                FD_CLR(sd, &readfds);
                FD_CLR(sd, &tmpReadfds);
                // max_sd-=1;
                clientSockets[i] = 0;
            }
           
        }
    }
    return 0;
}

int Server::getServersCount()
{   
    int p = 8000;
    std::map<int, Config*>::iterator it;
    for (it = configs_map.begin(); it != configs_map.end(); ++it)
    {
        it->second->_port = p++;
    }
    std::vector<int> unique_ports;
    for (it = configs_map.begin(); it != configs_map.end(); ++it)
    {
        /* Check unique ports and add to vector */
        if(std::find(unique_ports.begin(), unique_ports.end(), it->second->_port) == unique_ports.end())
        {
            std::cout << "Port Number - " << (it->second->_port) << std::endl;
        }
    }
    return 0;
}
