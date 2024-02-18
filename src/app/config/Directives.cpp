#include "Directives.hpp"

Directives::Directives() {}

Directives::~Directives() {}


void Directives::add_directives(std::pair<std::string,  std::vector<std::string> > &p)
{
    if(p.first == "autoindex")
        _autoindex = (p.second)[0];
    else if(p.first == "index")
    {
        for(int i = 0; i < p.second.size(); i++)
            _index.push_back(p.second[i]);
    }
    else if (p.first == "root")
        _root = p.second[0];
    else if (p.first == "upload_path")
        _upload_path = p.second[0];  
    else if(p.first == "client_max_body_size")
        _client_max_body_size = (p.second)[0];
    else if (p.first == "cgi")
        _cgi = p.second[0];
    else if(p.first == "return")
        _return = (p.second)[0];
    else if(p.first == "methods")
    {
        for(int i = 0; i < p.second.size(); i++)
            _methods.push_back(p.second[i]);
    }
    else if(p.first == "error_page")
    {
        std::pair<std::string, std::string> elem;
        elem.first = p.second[0];
        elem.second = p.second[1];
        _error_page.push_back(elem);
    }
    // directives.push_back(p);
}



std::string  const& Directives::getAutoindex() const
{
    return _autoindex;
}

std::string  const & Directives::getRoot() const
{  
    return _root;
}

std::string  const & Directives::getClient_max_body_size() const
{   
    return _client_max_body_size;
}

std::string  const & Directives::getUpload_path() const
{
    
    return _upload_path;
}

std::vector<std::pair<std::string, std::string> >  const & Directives::getError_page() const
{
    return _error_page;
}

std::vector<std::string>  const & Directives::getMethods() const
{
    return _methods;
}

std::vector<std::string>  const & Directives::getIndex() const
{
    
    return _index;
}

std::string  const & Directives::getCgi() const
{
    return (_cgi);
}

std::string  const & Directives::getReturn() const
{
    return (_return);
}

void Directives::printDirective() const {
    std::cout << "---- Directive ---- " << std::endl;
   
    std::cout << "_autoIndex: " << _autoindex << std::endl;
    std::cout << "_index: ";
    for (int j = 0; j < _index.size(); j++) {
        std::cout << _index[j] << " "; 
    }
    std::cout << "\n_root: " << _root << std::endl;
    std::cout << "_upload_path: " << _upload_path << std::endl;
    std::cout << "_client_max_body_size: " << _client_max_body_size << std::endl;
    for (int j = 0; j < _error_page.size(); j++)
    {
        std::cout << "_error_page: (" << _error_page[j].first << ", " << _error_page[j].second << ")" << std::endl;
    }
    std::cout << "_methods: ";
    for (int j = 0; j < _methods.size(); j++)
        std::cout << this->_methods[j] << " ";

    std::cout << std::endl;
    std::cout << "_cgi: " << _cgi << std::endl;
    std::cout << "_return: " << _return << std::endl;
}