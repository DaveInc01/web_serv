#ifndef IRESPONSEPARSER_HPP
#define IRESPONSEPARSER_HPP

#include "request/RequestParser.hpp"

class IResponseParser{
public:
    RequestParser getRequest();
protected:
    RequestParser request;
    std::string buffer;
};

#endif