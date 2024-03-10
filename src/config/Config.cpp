#include "AllConfigs.hpp"
#include "Config.hpp"

Config::Config() : _listen("localhost")
{
}

Config::~Config()
{
   std::vector<std::pair<std::string, Directives *> >::iterator itb = _locations.begin();
   while (itb != _locations.end())
   {
        delete itb->second;
        *itb++;

   }
}

void Config::fillLocations()
{
    for (int i = 0; i < _locations.size(); i++)
    {
        if (_locations[i].second->_autoindex.empty())
            _locations[i].second->_autoindex = this->_autoindex;
        if (_locations[i].second->_root.empty())
            _locations[i].second->_root = this->_root;
        if (_locations[i].second->_index.empty())
            _locations[i].second->_index = this->_index;
        if (_locations[i].second->_client_max_body_size.empty())
            _locations[i].second->_client_max_body_size = this->_client_max_body_size;
        if (_locations[i].second->_methods.empty())
            _locations[i].second->_methods = this->_methods;
        if (_locations[i].second->_upload_path.empty())
            _locations[i].second->_upload_path = this->_upload_path;
        if (_locations[i].second->_error_page.empty())
            _locations[i].second->_error_page = this->_error_page;
        if (_locations[i].second->_return.empty())
            _locations[i].second->_return = this->_return;
        if (_locations[i].second->_cgi.empty())
            _locations[i].second->_cgi = this->_cgi;
    }
};

void Config::add_locations(std::pair<std::string, Directives *> p)
{
    _locations.push_back(p);
}

void Config::add_listen(std::pair<std::string,  std::vector<std::string> > &p)
{
    if(p.first == "listen") {
        _listen = p.second[0];
        this->sum_func();
    }
}

void Config::add_servername(std::pair<std::string,  std::vector<std::string> > &p)
{
    if(p.first == "server_name")
        _server_name = p.second;
}


std::vector<std::pair<std::string, Directives *> > const & Config::get_locations() const
{
    return (_locations);
}


std::string const &Config::getListen() const
{
       return (_listen);
}

std::vector<std::string> const &Config::getServerName() const
{
    return _server_name;
}

void Config::printConfig() const {
    std::cout << "------- Config members ------- \n";
    std::cout << "_listen: " << this->_listen << std::endl;
    std::cout<< "_host " << _host << std::endl;
    std::cout<< "_port " << _port << std::endl;
    std::cout << "_server_name: ";
    for (int i = 0; i < this->_server_name.size(); i++)
        std::cout << this->_server_name[i] << " ";
    std::cout << "\n_autoIndex: " << this->_autoindex << std::endl;
    std::cout << "_index: ";
    for (int j = 0; j < this->_index.size(); j++) {
            std::cout << this->_index[j] << " "; 
    }
    std::cout << "\n_root: " << this->_root << std::endl;
    std::cout << "_upload_path: " << this->_upload_path << std::endl;
    std::cout << "_client_max_body_size: " << this->_client_max_body_size << std::endl;
    for (int j = 0; j < this->_error_page.size(); j++)
    {
        std::cout << "_error_page: (" << this->_error_page[j].first << ", " << this->_error_page[j].second << ")" << std::endl;
    }
    std::cout << "_methods: ";
    for (int j = 0; j < this->_methods.size(); j++)
        std::cout << this->_methods[j] << " ";

    std::cout << std::endl;
    std::cout << "_cgi: " << this->_cgi << std::endl;
    std::cout << "_return: " << this->_return << std::endl;

}

void Config::sum_func()
{
    size_t pos = _listen.find(":");
    _host = _listen.substr(0, pos);
    std::string str = _listen.substr(pos + 1);
    _port = atoi(str.c_str());
}

