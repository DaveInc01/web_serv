#include "IResponseParser.hpp"

int IResponseParser::setCorrespondingLocation()
{
    Config* corresponding_server = this->getCorrespondingServer();
    // Directives* confDirective = corresponding_server;
    checkDefaultLocation(corresponding_server);
    this->corresponding_location = getCorrespondingLocation(corresponding_server);
    this->setServeRoot();
    return 0;
}

int  IResponseParser::checkDefaultLocation(Config* config){
    std::vector<std::pair<std::string, Directives*> >::iterator it = config->_locations.begin();
    for (; it != config->_locations.end(); ++it)
    {
        if(it->first == "/")
        {
            this->have_def_location = 1;
            return (1);
        }
    }
    this->have_def_location = 0;
    return (0);
}

Directives *IResponseParser::getCorrespondingLocation(Config* config){
    std::string url_location = this->request.getRoute();
    int index = -1;
    size_t find_slash = 0;
    while(url_location.find("/") != std::string::npos)
    {
        index = findInVect(url_location, config);
        if(index == -1)
        {
            find_slash = url_location.rfind("/");
            url_location = url_location.substr(0, find_slash);
        }
        else{
            /* The location block was found from configs */
            this->location_class_path = config->_locations.at(index).first;
            return (config->_locations.at(index).second);
        }
    }
    this->location_class_path = "/";
    if(have_def_location)
    {
        /* set Location / as default */
        index = findInVect("/", config);
        return (config->_locations.at(index).second);
    }
    /* set Config* as default */
    Directives* defaultDirective = config;
    return  defaultDirective;
}

Config *IResponseParser::getMatchedServerName(std::vector<Config *> same_ports, std::string req_host_name)
{
    std::vector<int> name_positions;
    std::vector<std::string>::iterator servNameIt;
    /* comparing the req_host_name with the server_names of Configs */
	for (std::vector<Config *>::iterator confIt = same_ports.begin(); confIt != same_ports.end(); ++confIt)
    {
		if((*confIt)->_listen == req_host_name)
		{
			return (same_ports.at((confIt) - same_ports.begin()));
		}
	}
    for (std::vector<Config *>::iterator confIt = same_ports.begin(); confIt != same_ports.end(); ++confIt)
    {
        servNameIt = std::find((*confIt)->_server_name.begin(), (*confIt)->_server_name.end(), req_host_name);
        if (servNameIt != (*confIt)->_server_name.end())
            name_positions.push_back(servNameIt - (*confIt)->_server_name.begin());
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
            matched_chars = longestCommonPrefix(req_host_name, (*confIt)->_server_name);
            name_positions.push_back(matched_chars);
        }

        pos = findIndexOfMax(name_positions);
    }
    return(same_ports.at(pos));
}

Config* IResponseParser::getCorrespondingServer()
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
        return getMatchedServerName(same_ports, this->request.getHost());
}

int IResponseParser::findInVect(std::string url_location, Config * config)
{
    std::vector<std::pair<std::string, Directives*> >::iterator it = config->_locations.begin();
    for(; it != config->_locations.end(); ++it)
    {
        if(it->first == url_location)
            return (it - config->_locations.begin());
    }
    return -1;
}

/* set the location PATH(serve_root) which should serve the client */
int IResponseParser::setServeRoot(){
    std::string url_location = this->request.getRoute();
    size_t cut_from = url_location.find(this->location_class_path);
    if(cut_from != std::string::npos)
    {   
       try{
            this->serve_root = url_location.substr(location_class_path.length(), url_location.length());
            /* add url location / at first (/ + favicon.ico) */
            if(this->serve_root[0] != '/')
                this->serve_root.insert(0, 1, '/');
            /* if root is ending with '/' remove it (www/tmp/<-) */
            // if(this->corresponding_location->_root[this->corresponding_location->_root.length() - 1] == '/')
            //     this->corresponding_location->_root[this->corresponding_location->_root.length() - 1] = '\0';
            this->serve_root = this->corresponding_location->_root + this->serve_root;
        }
       catch(std::exception &e)
        {
           std::cout << e.what() << std::endl;
        }
    }
    return 0;
}

void IResponseParser::setCgiPId(int pid)
{
    this->cgiPID = pid;
}

void IResponseParser::setCgiStartTime() {
    this->cgiStartTime = time(NULL);
};

std::string IResponseParser::getServerRoot()
{
    return this->serve_root;
}



// int IResponseParser::cutResponse(int from)
// {
// 	this->_response = this->_response.substr(from, (this->_response.length() - from));
// 	return 0;
// }
