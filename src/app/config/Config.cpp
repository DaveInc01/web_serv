#include "AllConfigs.hpp"
#include "Config.hpp"

Config::Config()
{
    
}


Config::~Config()
{

}

void Config::add_locations(std::pair<std::string, Directives> &p)
{
    _locations.push_back(p);
}



std::vector<std::pair<std::string, Directives> > const & Config::get_locations() const
{
    return (_locations);
}


