#ifndef RESPONSEPARSER_HPP
#define RESPONSEPARSER_HPP

#include "IResponseParser.hpp"

class ResponseParser : public IResponseParser
{
public:
    ResponseParser(RequestParser, std::map<int, Config *>);
    ResponseParser();
    int launchResponse();
	std::string 	_response; // The final response string
};

#endif