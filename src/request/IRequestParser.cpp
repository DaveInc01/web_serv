#include "IRequestParser.hpp"

std::string IRequestParser::getUrl()
{
    return this->url;
}

std::string IRequestParser::getMethod()
{
    return this->method;
}

std::string IRequestParser::getRoute()
{
    return this->route;
}

std::string IRequestParser::getQueryString(){
     return this->query_string;
}

std::string IRequestParser::getContentType()
{
    return this->content_type;
}

std::string IRequestParser::getContentLength()
{
    return this->content_length;
}

std::string IRequestParser::getPostReqBody()
{
    return this->post_req_body;
}

std::string IRequestParser::getHttpReq()
{
    return this->http_req;
}

int IRequestParser::getIsReqEnd()
{
    return this->is_req_end;
}

std::string IRequestParser::getHost()
{
    return this->host;
}

int     IRequestParser::getPort()
{
    return this->port;
}

std::string IRequestParser::getPort_str()
{
    return this->port_str;
}

std::string IRequestParser::getPostReqFilename(){
    return this->post_req_filename;
}

void IRequestParser::setClientIp(char *cl_ip)
{
    this->client_ip = cl_ip;
}

char *IRequestParser::getClientIp()
{
    return this->client_ip;
}
