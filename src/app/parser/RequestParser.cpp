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
   int char_index = 0;
   int line_index = 0;
   std::string line;
   
   while ((line = RequestParser::getLine(char_index)).length() > 0)
   {
      if(line_index == 0)
      {
         if(parseMethod(line) == -1)
            throw("Unknown request method");

         std::cout << line;
         this->request.insert(request.end(), {"start", rtrim(line)});
      }
      if (line_index >= 1)
      {
         if(line.find(':') != std::string::npos)
         {
            std::pair<std::string, std::string> p_line = ft_split(line, ':');
            request.insert(request.end(), p_line);
         }
         else if(char_index == buff_len)
            std::cout << "end of request\n";
         else {
            std::cout << "char_index - " << char_index << "\nbuff_len - " << buff_len << std::endl;
            //trying to get the post method content
            std::ofstream myfile;
            myfile.open ("example.txt");
            std::cout << "file was opened\n";
            while ((line = RequestParser::getLine(char_index)).length() > 0)
            {
               //sort the body in the map
               myfile << line;
               std::cout <<"content - "<< line;
            }
            myfile.close();
         }
         std::cout << "line-" << line;
      }
      line_index++;
   }
   
   // std::map<std::string, std::string>::iterator it = request.begin();
   // while(it != request.end())
   // {
   //    std::cout << it->first << it->second << std::endl;
   //    ++it;
   // }
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
