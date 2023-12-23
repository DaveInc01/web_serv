#ifndef IREQUESTPARSER
#define IREQUESTPARSER

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class IRequestParser {
protected:
   std::string url;
   std::string method;
   std::string route;
   std::string query;
   std::string content_type;
   std::string content_length;
   
   std::string buff;
   std::string http_v;
   std::map<std::string, std::string> request;
   int parse_count;
   int buff_len;
   int header_finish;
   std::string post_req_filename;
   std::string post_req_body;


   // std::vector<std::string> post_body;
};


std::string& rtrim(std::string& s);
std::string& ltrim(std::string& s);
std::pair<std::string, std::string> ft_split(std::string s, char del);

#endif