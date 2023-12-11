#ifndef REQUESTPARSER
#define REQUESTPARSER

#include "IRequestParser.hpp"



class RequestParser : public IRequestParser {
public:
      RequestParser(std::string, int);

      int parseUrl();
      int parseMethod(std::string);
      int parseRoute();
      int parseQuery();
      int parseConent();
      int launchParse();
      std::string getLine(int &);

      std::map<std::string, std::string> request;
      int buff_len;
};


#endif