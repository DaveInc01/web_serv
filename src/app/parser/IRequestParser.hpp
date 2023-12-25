#ifndef IREQUESTPARSER
#define IREQUESTPARSER

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class IRequestParser {
public:
   std::string getUrl();
   std::string getMethod();
   std::string getRoute();
   std::string getContentType();
   std::string getContentLength();
   std::string getPostReqBody();

protected:
   std::string method;
   std::string route;
   std::string http_v;
   std::string url;
   std::string content_type;
   std::string content_length;
   int         content_length_int;
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
   std::map<std::string, std::string>  request;
};


std::string& rtrim(std::string& s);
std::string& ltrim(std::string& s);
std::pair<std::string, std::string> ft_split(std::string s, char del);

#endif