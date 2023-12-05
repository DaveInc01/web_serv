#ifndef REQUESTPARSER
#define REQUESTPARSER

#include "IRequestParser.hpp"

class RequestParser : protected IRequestParser {
      RequestParser(char *)

      int parseUrl();
      int parseMethods();
      int parseRoute();
      int parseQuery();
      int parseConent();

}

#endif