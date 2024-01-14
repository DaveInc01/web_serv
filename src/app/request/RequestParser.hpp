#pragma once

#include "IRequestParser.hpp"

class RequestParser : public IRequestParser {
public:
      RequestParser();

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
      
      std::string getLine(int &);
};
