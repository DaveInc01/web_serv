#include "IResponseParser.hpp"

int IResponseParser::setCorrespondingLocation()
{
    for (std::map<int, Config *>::iterator it = this->configs_map.begin(); it != this->configs_map.end(); ++it)
    {
        std::cout << "Port NUMBER - " << it->second->_port << std::endl;
    }
    return 0;
}

Config *IResponseParser::getMatchedServerName(std::vector<Config *> same_listen, std::string req_host)
{
    for (std::vector<Config *>::iterator it = same_listen.begin(); it != same_listen.end(); ++it)
    {
        std::cout << (*it)->_port << std::endl;
    }
    return NULL;
}

int IResponseParser::getCorresponding_server()
{
    std::string req_host = this->request.getHost();
    std::vector<Config *> same_listen;
    for (std::map<int, Config *>::iterator it = this->configs_map.begin(); it != this->configs_map.end(); ++it)
    {
        if(req_host == it->second->_listen)
            same_listen.push_back(it->second);
    }
    if(same_listen.size() > 0)
    {
        getMatchedServerName(same_listen, req_host);
        // if(same_listen.size() == 1)
    }
    return 0;
}