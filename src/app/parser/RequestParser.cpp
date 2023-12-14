#include "RequestParser.hpp"

RequestParser::RequestParser(std::string req, int len){
   buff = req;
   buff_len = len;
}

int RequestParser::setValue(std::string key, std::string &obj_property)
{
   std::string value;
   if(request.find(key) != request.end())
   {
      value = request.find(key)->second;
      obj_property = value;
      return (0);
   }
   return -1;
}

int RequestParser::setProperties(){
   parseRoute();
   if(this->method == "POST")
   {
      setValue("Content-Length", this->content_length);
      setValue("Content-Type", this->content_type);
   }
   // std::cout << "Content-Length_my - " << this->content_length <<
   // std::endl << "Content-type_my - " << this->content_type << std::endl;
   // std::vector<std::string>::iterator it = post_body.begin();
   // while(it != post_body.end())
   // {
   //    std::cout << *it;
   //    ++it;
   // }
   return 0;
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
   std::string line = this->request.find("start")->second;
   int start = line.find('/');
   int end = line.find(' ', start);
   if (start != std::string::npos && end != std::string::npos)
   {
      try{
         this->route = line.substr(start, (end - start));
      }
      catch(std::exception &e)
      {
         std::cout << e.what() << std::endl; 
      }
      parseHttpVersion(line);
   }
   return 0;
}

int RequestParser::parseHttpVersion(std::string line)
{
   int sub_start = line.rfind('/');
   if(sub_start != std::string::npos)
   {
      try{
         this->http_v = line.substr(sub_start + 1, (line.size() - sub_start));;
      }
      catch(std::exception &e){
         std::cout << e.what() << std::endl;
      }
   }
   return 0;
}
int RequestParser::parseQuery()
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
         this->request.insert(std::make_pair("start", line));
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
            // set obj preoperties values
            setProperties();
            std::cout << "char_index - " << char_index << "\nbuff_len - " << buff_len << std::endl;
            //trying to get the post method content
            std::ofstream myfile;
            size_t sub_start = content_type.rfind('/');
            // ++sub_start;
            std::string type;
            if(sub_start != std::string::npos)
            {
               type = content_type.substr(sub_start, content_type.size() - sub_start);
               type[0] = '.';
            }
            myfile.open ("example" + type);
            std::cout << "file name - " << "example" << type << std::endl;
            std::cout << "file was opened\n";
            while ((line = RequestParser::getLine(char_index)).length() > 0)
            {
               //sort the body in the map
               // this->post_body.push_back(line);
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
