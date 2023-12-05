#ifndef IREQUESTPARSER
#define IREQUESTPARSER

#include <map>
#include <iostream>
#include <string>

class IRequestParser {
protected:
   char* url;
   char* method;
   char* route;
   char* query;
   char* content;
   std::string buff;
};

#endif