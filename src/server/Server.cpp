#include "Server.hpp"

#define SEND_SIZE 1000

Server::Server( std::map<int, Config*> configs)
{
    this->configs_map = configs;
    this->MAX_CLIENTS = 5;
    buff_size = 100000;
    max_sd = 0;
    /* find unique ports and init to servers_count */
    servers_count = getServersCountFromConf();
    server_sockets.resize(servers_count);
    client_sockets.resize(MAX_CLIENTS);
}

int Server::upServer(int serv_socket, int port)
{
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    inet_pton(AF_INET, "0.0.0.0", &srv.sin_addr);
    memset(&(srv.sin_zero), 0, 8);
    addrlen = sizeof(srv);
    int l = 1;
    setsockopt(serv_socket, SOL_SOCKET, SO_REUSEADDR, &l, sizeof(l));
    if (fcntl(serv_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) < 0)
    {
        throw("Fcntl error");
    }
    int nRet = bind(serv_socket, (sockaddr*)&srv, sizeof(sockaddr));
    if(nRet < 0)
    {
        std::cout << "Fail to bind to loacal port\n";
        exit(EXIT_FAILURE);
    }

    nRet = listen(serv_socket, MAX_CLIENTS);
    if(nRet < 0)
    {
        std::cout << "max count of connections was full - " << MAX_CLIENTS << std::endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}

int Server::setServSockets()
{
    int sd;
    for (int i = 0; i < servers_count; i++)
    {
        FD_SET(server_sockets.at(i), &readfds);
        if(server_sockets.at(i) > max_sd)
            max_sd = server_sockets.at(i);
    }

    for(int i = 0; i < MAX_CLIENTS; i++){
        sd = client_sockets.at(i);
        if(sd > 0)
        {
            FD_SET(sd, &readfds);
        }
        if(sd > max_sd)
        {
            max_sd = sd;
        }
    }
    return 0;
}

void Server::httpIO()
{
    char buff[buff_size];
    int activity, sd;
    int newSocket = 0;

    tmpReadfds = readfds;
    tmpWritefds = writefds;

    do {
        activity = select(max_sd + 1, &tmpReadfds, &tmpWritefds, NULL, NULL);
    } while (activity == -1);
    for(int i = 0; i < servers_count; i++)
    {
        if (FD_ISSET(server_sockets.at(i), &tmpReadfds) != 0)
        {
            /* new conection has come */ 
            if ((newSocket = accept(server_sockets.at(i), (struct sockaddr*)&srv, (socklen_t*)&addrlen)) < 0) {
                continue;
            }
        }
    }
    if(FD_ISSET(newSocket, &readfds) == 0)
    {
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if(client_sockets.at(i) == 0)
            {
                FD_SET(newSocket, &readfds);
                fcntl(newSocket, F_SETFL, O_NONBLOCK);
                client_sockets.at(i) = newSocket;
                std::pair<int, RequestParser> clients_req_elem;
                clients_req_elem.first = newSocket;
                clients_req_elem.second = RequestParser();
                clients_req_elem.second.setFd(newSocket);
                clients_req_elem.second.setClientIp(inet_ntoa(srv.sin_addr));
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
        sd = client_sockets.at(i);
        if(FD_ISSET(sd, &tmpReadfds))
        {
            valread = recv(sd, buff, sizeof(buff) - 1, 0);
            buff[valread] = '\0';
            if (valread > 0)
            {
                std::string strBuff(buff, valread);
                clientsReq.at(sd).launchParse(strBuff, strBuff.size());

                if(clientsReq.at(sd).getIsReqEnd())
                {
                    FD_SET(sd, &writefds);
                    FD_CLR(sd, &readfds);
                    FD_CLR(sd, &tmpReadfds);
                    std::pair<int, ResponseParser> clients_resp_elem;
                    clients_resp_elem.first = sd;
                    clients_resp_elem.second = ResponseParser(clientsReq.at(sd), this->configs_map);
                    clientsResp.insert(clients_resp_elem);
                }
                FD_ZERO(&tmpReadfds);
                // FD_ZERO(&tmpWritefds);
                continue;
            }
            if (valread == -1 || valread == 0) {
                FD_CLR(sd, &readfds);
                clientsReq.erase(sd); 
                continue;
            }
        }
        
        else if(FD_ISSET(sd, &tmpWritefds))
        {
            int res;
			if(clientsResp.at(sd)._response.length() > SEND_SIZE)
			{
				/* Response is bigger than SEND_SIZE */
            	res = send(sd, clientsResp.at(sd)._response.c_str(), SEND_SIZE,0);
                if (res == -1 || res == 0) {
                    FD_CLR(sd, &writefds);
                    clientsReq.erase(sd); 
                    clientsResp.erase(sd); 
                    continue;
                }
				clientsResp.at(sd)._response = clientsResp.at(sd)._response.substr(SEND_SIZE, (clientsResp.at(sd)._response.length() - SEND_SIZE));
				// FD_ZERO(&tmpWritefds);
				break ;
			}
			else{
            	res = send(sd, clientsResp.at(sd)._response.c_str(), clientsResp.at(sd)._response.size(), 0);
                if (res == -1 || res == 0) {
                    FD_CLR(sd, &writefds);
                    clientsReq.erase(sd); 
                    clientsResp.erase(sd); 
                    continue;
                }
				getpeername(sd, (struct sockaddr*)&srv, (socklen_t*)&addrlen);
				std::cout << "Host disconnected, ip " << inet_ntoa(srv.sin_addr) << " , port " << ntohs(srv.sin_port) << std::endl;
				
				clientsReq.erase(sd); 
				clientsResp.erase(sd);
				close(sd);

				FD_CLR(sd, &writefds);
				FD_CLR(sd, &tmpWritefds);
				FD_ZERO(&tmpWritefds);
				client_sockets.at(i) = 0;
			}
        }  
    }
}

int Server::launchServer()
{
    for (int i = 0; i < servers_count; i++)
    {
        server_sockets[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (server_sockets.at(i) < 0)
        {
            throw ("The socket not oppened");
        }
        else{
            /* Bind, listen, etc.. */
            upServer(server_sockets[i], unique_ports.at(i));
            std::cout << "socket has opened on port " << unique_ports.at(i) << std::endl;
        }
    }

    for(int i = 0; i < MAX_CLIENTS; i++){
        client_sockets.at(i) = 0;
    }

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&tmpReadfds);
    FD_ZERO(&tmpWritefds);
    /* Set server Sockets to &readfds */
    setServSockets();

    
    while (true)
    {   /*The all requset and response processes*/
        httpIO();
    }
    return 0;
}

int Server::getServersCountFromConf()
{   
    std::map<int, Config *>::iterator it;
    for (it = configs_map.begin(); it != configs_map.end(); ++it)
    {
        /* Check unique ports and add to vector from Server class */
        if(std::find(unique_ports.begin(), unique_ports.end(), it->second->_port) == unique_ports.end())
        {
            unique_ports.push_back(it->second->_port);
        }
    }
    return (int) unique_ports.size();
}

int Server::getCorrespondingLoc(RequestParser &request)
{
    std::cout << request.getRoute() <<  std::endl;

    return (0);
}