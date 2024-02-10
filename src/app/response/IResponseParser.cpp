#include "IResponseParser.hpp"

int IResponseParser::setCorrespondingLocation()
{
    Config* corresponding_server = this->getCorresponding_server();
    std::cout << "Queue - " << corresponding_server->queue << std::endl;
    for (std::vector<std::pair<std::string, Directives> >::iterator it = corresponding_server->_locations.begin(); it != corresponding_server->_locations.end(); ++it)
    {
        std::cout << "Location - "<< it->first << std::endl;
    }
    Directives* confDirective = corresponding_server;
    return 0;
}


Config *IResponseParser::getMatchedServerName(std::vector<Config *> same_ports, int req_port, std::string req_host_name)
{
    std::vector<int> name_positions;
    std::vector<std::string>::iterator servNameIt;
    /* comparing the req_host_name with the server_names of Configs */
    for (std::vector<Config *>::iterator confIt = same_ports.begin(); confIt != same_ports.end(); ++confIt)
    {
        servNameIt = std::find((*confIt)->_server_names.begin(), (*confIt)->_server_names.end(), req_host_name);
        if (servNameIt != (*confIt)->_server_names.end())
            name_positions.push_back(servNameIt - (*confIt)->_server_names.begin());
        else
            name_positions.push_back(-1);
    }
    /* find the right config position with full server_name matching */
    int pos = findPosition(name_positions);
    if(pos != -1)
        return(same_ports.at(pos));
    /* find the right config position with part of server_name matching */
    else{
        name_positions.clear();
        pos = 0;
        int matched_chars = 0;
        for (std::vector<Config *>::iterator confIt = same_ports.begin(); confIt != same_ports.end(); ++confIt)
        {
            matched_chars = longestCommonPrefix(req_host_name, (*confIt)->_server_names);
            // std::cout << "matched chars - " << matched_chars << std::endl;
            name_positions.push_back(matched_chars);
        }

        pos = findIndexOfMax(name_positions);
    }
    return(same_ports.at(pos));
    // return same_ports.at(0);
}

Config* IResponseParser::getCorresponding_server()
{
    int req_port = this->request.getPort();
    std::vector<Config *> same_ports;
    for (std::map<int, Config *>::iterator it = this->configs_map.begin(); it != this->configs_map.end(); ++it)
    {
        if(req_port == it->second->_port)
            same_ports.push_back(it->second);
    }
    // for(std::vector<Config *>::iterator it = same_ports.begin(); it != same_ports.end(); it++)
    // {
    //     std::cout << "The port is " << (*it)->_port << std::endl;
    // }
    if (same_ports.size() == 1)
        return(same_ports.at(0));
    else
        return getMatchedServerName(same_ports, req_port, this->request.getHost());
}



