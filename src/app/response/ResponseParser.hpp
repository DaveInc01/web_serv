#ifndef RESPONSEPARSER_HPP
#define RESPONSEPARSER_HPP

#include "IResponseParser.hpp"

class ResponseParser : public IResponseParser
{
public:
    ResponseParser(){};
    ResponseParser(RequestParser, std::map<int, Config *>);
    int launchResponse();
    int checkIsAloowedMethod();
    int generateGetResponse();
    int generatePostResponse();
    int generateDeleteResponse();
    std::string checkErrorFromConf(int);
    int setErrorMap();
    void setResponse(std::string);
    std::string     _header;
	std::string 	_response; // The final response string
};

#endif