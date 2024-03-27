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

class Config : public Directives
{
public:
    std::vector<std::string> _server_name;
    std::string _listen;
    std::string _host;
    int         _port;
    std::vector<std::pair<std::string, Directives *> > _locations;
    void sum_func();

public:
    Config();
    ~Config();

    void fillLocations();
    void add_locations(std::pair<std::string, Directives *> p);

    std::vector<std::pair<std::string, Directives *> >  const & get_locations() const;
    std::string  const& getListen() const;
    std::vector<std::string> const&  getServerName() const;

    void add_listen(std::pair<std::string, std::vector<std::string> > &p);
    void add_servername(std::pair<std::string,  std::vector<std::string> > &p);
    void printConfig() const;


   
   
};







#endif