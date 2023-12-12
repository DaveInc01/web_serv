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
         {
            std::cout << line;
            throw("Unknown request method");
         }
         this->request["start"] = line;
         std::cout << line;
      }
      if (i >= 1)
      {
         if(line.find(':') != std::string::npos)
            std::pair<std::string, std::string> p = ft_split(line, ':');
         else{
            //trying to get the postmethod content
            while ((line = RequestParser::getLine(index)).length() > 0)
            {
               //sort the body in the map
               std::cout << line;
            }
         }
         std::cout << "line-" << line;
         if(ltrim(rtrim(line)).size() < 1)
            std::cout << "the empty line ----------\n";
         // if(p != NULL)
            // this->request.insert(*p);
      }
      // std::cout << line << std::endl;
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
      {
         line.push_back(buff[index]);
         index++;
         break;
      }
   }
   return (line);
}
