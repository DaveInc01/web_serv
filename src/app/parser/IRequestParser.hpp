#ifndef IREQUESTPARSER
#define IREQUESTPARSER

#include <map>
#include <iostream>
#include <string>

class IRequestParser {
protected:
   std::string url;
   std::string method;
   std::string route;
   std::string query;
   std::string content;
   std::string buff;
};


std::string& rtrim(std::string& s);
std::string& ltrim(std::string& s);
std::pair<std::string, std::string> ft_split(std::string s, char del);

#endif