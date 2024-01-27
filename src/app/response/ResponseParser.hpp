#ifndef RESPONSEPARSER_HPP
#define RESPONSEPARSER_HPP

#include "IResponseParser.hpp"

class ResponseParser : public IResponseParser
{
public:
    ResponseParser(RequestParser);
    ResponseParser();
    int launchResponse();
    int    setCorrespondingLocation();
};

#endif