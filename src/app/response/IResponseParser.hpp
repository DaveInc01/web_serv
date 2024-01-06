#ifndef IRESPONSEPARSER_HPP
#define IRESPONSEPARSER_HPP

#include "request/RequestParser.hpp"

class IResponseParser{
public:
    int             setFinish();
protected:
    RequestParser   request;
    
    std::string     buffer;
    std::string     content_type;
    std::string     server_name;
    std::string     date;
    std::string     response_body;
    int             content_length;
    int             is_finish;
    int             status_code;
    int             http_version;
};

#endif