#ifndef REQUESTPARSER
#define REQUESTPARSER

#include "IRequestParser.hpp"



class RequestParser : public IRequestParser {
public:
      RequestParser(){};

      int parseUrl();
      int parseMethod(std::string);
      int parseRoute();
      int parseHttpVersion(std::string);
      int parseQuery();
      int parseConent();
      int launchParse( std::string, int );
      int setProperties();
      int setValue(std::string, std::string &);
      std::string getLine(int &, std::string);

      std::map<std::string, std::string> request;
      static int parse_count;
      int buff_len;
      std::string post_req_filename;
};


#endif