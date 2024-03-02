#ifndef ERRORS_HPP
#define ERRORS_HPP
#include "server/Server.hpp"

class Errors
{
private:
    std::map<int, std::string>	_errors;
    std::string _def_error_response;
public:
    Errors(int status, std::vector<std::pair<std::string, std::string> > conf_error);
    ~Errors();
    int setErrorMap();
    int setDefaultErrorResponse(int);
    std::string getErrorResponse();
};



#endif