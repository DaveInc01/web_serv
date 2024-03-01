#ifndef RESPONSEPARSER_HPP
#define RESPONSEPARSER_HPP

#include "IResponseParser.hpp"

class ResponseParser : public IResponseParser
{
public:
    ResponseParser(RequestParser, std::map<int, Config *>);
    ResponseParser();
    int launchResponse();
    int checkIsAloowedMethod();
    int getResponse();
    int postResponse();
    int deleteResponse();
    int sendNotAllowed();
    int sendErrorStatus(int);
    std::string checkErrorFromConf(int);
    int setErrorMap();
    std::string     _header;
	std::string 	_response; // The final response string
};

#endif