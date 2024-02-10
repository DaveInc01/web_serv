#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 
#include "Directives.hpp"

// typedef std::vector<std::pair<std::string,  std::vector<std::string>>> Vec;

class Config : public Directives
{
private:
    //Vec directives;
public:
    int         _port;
    std::string _host;
    std::string _listen;
    std::vector<std::string> _server_names;
    int queue;
    int sd;
    std::vector<std::pair<std::string, Directives> > _locations;

public:
    Config();
    ~Config();

    void add_locations(std::pair<std::string, Directives> &p);
    std::vector<std::pair<std::string, Directives> > const & get_locations() const;
};







#endif