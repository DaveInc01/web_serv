#include "RequestParser.hpp"

RequestParser::RequestParser(std::string req, int len){
   buff = req;
   buff_len = len;
}

int RequestParser::parseUrl()
{
   return  0;
}

int RequestParser::parseMethod()
{
   return 0;
}

int RequestParser::parseRoute()
{
   return 0;
}

int RequestParser::parseQuery()
{
   return 0;
}

int RequestParser::parseConent()
{
   return 0;
}

int RequestParser::launchParse()
{
   int index = 0;
   int i = 0;
   std::string line;
   
   while ((line = RequestParser::getLine(index)).length() > 0)
   {
      // if (line[i] == '\0')
      //    break;
      std::cout << line << std::endl;
   }
   return 0;
}

std::string RequestParser::getLine(int &index)
{
   std::string line = "";
   
   while ((buff[index] != '\n') && (index < buff_len))
   {
      line.push_back(buff[index]);
      index++;
   }
   index++;
   return line;
}
