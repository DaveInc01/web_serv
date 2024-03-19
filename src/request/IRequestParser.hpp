#pragma once

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include "config/AllConfigs.hpp"


class IRequestParser {
public:
   std::string getUrl();
   std::string getMethod();
   std::string getRoute();
   void        setQueryString();
   std::string getQueryString();
   std::string getContentType();
   std::string getContentLength();
   std::string getPostReqBody();
   std::string getHttpReq();
   std::string getHost();
   int         getIsReqEnd();
   int         getPort();
   std::string getPort_str();
   std::string getPostReqFilename();
   void        setClientIp(char*);
   char*       getClientIp();
protected:
   std::string method;
   std::string route;
   std::string query_string;
   std::string http_v;
   std::string url;
   int         port;
   std::string port_str;
   std::string host;
   std::string content_type;
   std::string content_length;
   unsigned long content_length_int;
   std::string transfer_encoding;
   std::string buff;
   std::string post_req_filename;
   std::string post_req_body;
   std::string unfinished_line;
   std::string http_req;
   int         buff_len;
   int         header_finish;
   int         header_line_finish;
   int         is_req_end;
   std::string boundary;
   char*       client_ip;
   
   std::map<std::string, std::string>  request;
};


std::string& rtrim(std::string& s);
std::string& ltrim(std::string& s);
std::pair<std::string, std::string> ft_split(std::string s, char del);
