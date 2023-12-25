#include "RequestParser.hpp"

RequestParser::RequestParser(){
   header_finish = 0;
   is_req_end = 0;
   header_line_finish = 1;
   content_length_int = 0;
};


int   RequestParser::setValue(std::string key, std::string &obj_property)
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

int   RequestParser::setProperties(){
   parseRoute();
   if(this->method == "POST")
   {
      setValue("Content-Type", this->content_type);
      if(setValue("Content-Length", this->content_length) != -1)
         this->content_length_int = atoi(content_length.c_str());


   }
   
   return 0;
}

int   RequestParser::parseUrl()
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
         this->request.insert(std::make_pair("start", line));
         return 0;
      }
      else
         i++;
   }
   return -1;
}

int   RequestParser::parseRoute()
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

int   RequestParser::parseHttpVersion(std::string line)
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



int   RequestParser::launchParse( std::string buff, int len )
{
   int char_index = 0;
   this->buff_len = len;
   this->buff = buff;
   this->http_req.append(buff);
   std::string line;

   while ((line = RequestParser::getLine(char_index)).length() > 0)
   {
      /* Header is exist */
      if(!header_finish)
      {
         /* Unit Line parse was finished */
         if(header_line_finish){
            /* Start line is empty */
            if(this->method.empty())
            {
               if(parseMethod(line) == -1)
                  throw("Unknown request method");
               // std::cout << "start - " << line;
            }
            /* End of request header */
            else if(line == "\r\n"){
               header_finish = 1;
               setProperties();
            }
            /* Common line of header with ':' */
            else{
               std::pair<std::string, std::string> p_line = ft_split(line, ':');
               request.insert(request.end(), p_line);
            }
         }
         else{
            this->unfinished_line = line;
            continue;
         }
      }
   }
   findReqEnd();


   // if(!(this->post_req_body.empty()))
   //    std::cout << this->post_req_body;

   
   return 0;
}


std::string RequestParser::getLine(int &index)
{
   std::string line = "";
      
   while (index < buff_len)
   {
      if(!header_finish)
      {
         if(!header_line_finish)
         {
            line = unfinished_line;
            line.push_back(this->buff[index]);
            unfinished_line.erase();
            this->header_line_finish = 1;
         }
         else
         {
            line.push_back(this->buff[index]);
         }
         if(this->buff[index] == '\n')
         {
            index++;
            return(line);
         }
      }
      else{
         post_req_body.push_back(this->buff[index]);
      }
      index++;
   }
   /* if header line was not finished */
   int line_len = line.length();
   if((line_len > 0) && (header_finish <= 0))
      if(line[line_len-1] != '\n')
         this->header_line_finish = -1;

   return (line);
}

int   RequestParser::findReqEnd()
{
   if(this->http_req.find("\r\n\r\n") != std::string::npos)
   {
      if(this->method != "POST")
         this->is_req_end = 1;  
      else{
         if(this->transfer_encoding != "chunked")
         {
            if(this->content_length_int > 0)
            {
               if(this->post_req_body.length() == content_length_int)
                  this->is_req_end = 1;
            }
         }
         else{
            /* create chuked req end logic */
         }
      }
   }
   return (0);
}