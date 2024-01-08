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
   int found;
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
      {
         if(parseMethod(line) == -1)
            throw("Unknown request method");
         this->request["0"] = line;
      }
      if (i >= 1)
      {
         std::cout<< "split after\n";
         std::pair<std::string, std::string> *p = ft_split(line, ':');
         // if(p != NULL)
            // this->request.insert(*p);
      }
      std::cout << line << std::endl;
      i++;
   }
   return 0;
}


std::string RequestParser::getLine(int &index)
{
   std::string line = "";
   
   while (index < buff_len)
   {
      line.push_back(buff[index]);
      index++;
      if(buff[index] == '\n')
         break;
   }
   return (rtrim(line));
}
