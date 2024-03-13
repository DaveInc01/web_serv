#pragma once

#include "IRequestParser.hpp"

class RequestParser : public IRequestParser {
public:
      RequestParser();
      RequestParser(int &);  
      int   parseUrl();
      int   parseMethod(std::string);
      int   parseRoute();
      int   parseHttpVersion(std::string);
      int   parseQuery();
      int   parseConent();
      int   launchParse( std::string, int );
      int   setProperties();
      int   setValue(std::string, std::string &);
      int   findReqEnd();
      int   getFd();
      void  setFd(int &);
      void parseMultipartFormData(const std::string&, const std::string&);
      std::string getLine(int &);
      int _fd;
};
