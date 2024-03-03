#ifndef ERRORS_HPP
#define ERRORS_HPP
#include "server/Server.hpp"

class Errors
{
private:
    std::map<int, std::string>	_status_codes;
    std::string error_response;
    std::string error_file_path;
public:
    Errors();
    Errors(int status, ResponseParser&);
    ~Errors();
    int setErrorMap();
    int setDefaultErrorResponse(int);
    std::string getPathFromConf(int status, ResponseParser&);
    std::string getErrorResponse();
    std::string getStatusText(int status_code);
    std::string getStatusLine(int status_code);
};

std::string concatStrings(std::string str1, std::string str2);

#endif