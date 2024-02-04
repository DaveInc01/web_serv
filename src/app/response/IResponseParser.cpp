#include "IResponseParser.hpp"

int IResponseParser::setCorrespondingLocation()
{
    this->getCorresponding_server();
    // for (std::map<int, Config *>::iterator it = this->configs_map.begin(); it != this->configs_map.end(); ++it)
    // {
    //     std::cout << "Port NUMBER - " << it->second->_port << std::endl;
    // }
    return 0;
}

Config *IResponseParser::getMatchedServerName(std::vector<Config *> same_ports, int req_port)
{
    for (std::vector<Config *>::iterator it = same_ports.begin(); it != same_ports.end(); ++it)
    {
        std::cout << (*it)->_port << std::endl;
    }
    return NULL;
}

int IResponseParser::getCorresponding_server()
{
    int req_port = this->request.getPort();
    std::vector<Config *> same_ports;
    for (std::map<int, Config *>::iterator it = this->configs_map.begin(); it != this->configs_map.end(); ++it)
    {
        // if(req_port == it->second->_port)
        //     same_ports.push_back(it->second);
    }
    if(same_ports.size() > 0)
    {
        getMatchedServerName(same_ports, req_port);
        // if(same_listen.size() == 1)
    }
    return 0;
}