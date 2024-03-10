#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>


typedef std::vector<std::pair<std::string,  std::vector<std::string> > > Vec;

class Directives 
{

public:
    Directives();
    ~Directives();
    
    void add_directives(std::pair<std::string,  std::vector<std::string> > &p);


   /* Getter functions */
public:
    std::string _autoindex;
    std::vector<std::string> _index;
    std::string _root;
    std::string _upload_path;
    std::string _client_max_body_size;
    std::vector<std::pair<std::string, std::string> > _error_page;
    std::vector<std::string> _methods;
    std::string _cgi;
    std::string _return;

    std::string const & getAutoindex() const;
    std::vector<std::string> const &  getIndex() const;
    std::string const &  getRoot() const;
    std::string const &  getUpload_path() const;
    std::string const &  getClient_max_body_size() const;
    std::vector<std::pair<std::string, std::string> > const &  getError_page() const;
    std::vector<std::string> const &  getMethods() const;
    std::string const &  getCgi() const;
    std::string const &  getReturn() const;
    
    void printDirective() const;

};







#endif