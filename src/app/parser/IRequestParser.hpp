#ifndef IREQUESTPARSER
#define IREQUESTPARSER

class IRequestParser {
protected:
   char* url;
   char* method;
   char* route;
   char* query;
   char* content;
   char* buff;
};

#endif