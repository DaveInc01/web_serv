#ifndef ERRORS_HPP
#define ERRORS_HPP
#include "server/Server.hpp"

class Errors
{
private:
    std::map<int, std::string>	_errors;
public:
    Errors(int);
    ~Errors();
    int setErrorMap();
    int setDefaultErrorResponse(int);
    std::string _def_error_response;
    std::string getErrorResponse();
};



#endif