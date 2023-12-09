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

#endif