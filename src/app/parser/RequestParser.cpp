#include "RequestParser.hpp"

RequestParser::RequestParser(std::string req, int len){
   buff = req;
   buff_len = len;
}

int RequestParser::parseUrl()
{
   return  0;
}

int RequestParser::parseMethod(std::string line)
{
   std::string reqMethods[3] = {"GET", "POST", "DELETE"};
   int i = 0;
   int found = std::string::npos;
   while(i < 3)
   {
      if(line.find(reqMethods[i]) == 0)
      {
         this->method = reqMethods[i];
         return 0;
      }
      else
         i++;
   }
   return -1;
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
      if(i == 0)
         parseMethod(line);
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
