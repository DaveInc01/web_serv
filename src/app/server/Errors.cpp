#include "server/Errors.hpp" 


/* For getting a map of status codes */
Errors::Errors(){
    setErrorMap();
}

/* For getting Full response of Error status codes */
Errors::Errors(int status, ResponseParser& response)
{
    setErrorMap();
    getPathFromConf(status, response);
    setDefaultErrorResponse(status);

}

std::string Errors::getPathFromConf(int status, ResponseParser& response){
    std::vector<std::pair<std::string, std::string> > conf_error = response.corresponding_location->getError_page();
    std::string status_str = std::to_string(status);
    for (std::vector<std::pair<std::string, std::string> >::iterator it = conf_error.begin(); it != conf_error.end(); ++it)
    {
        if(status_str == it->first)
        {
            // concat root and error path
            error_file_path = response.corresponding_location->getRoot();
            error_file_path = concatStrings(error_file_path, it->second);
            std::cout << "Error file path = " << error_file_path << std::endl;
            return error_file_path;
        }
    }
    return NULL;
}

int Errors::setErrorMap(){
    _status_codes[100] = "Continue";
	_status_codes[101] = "Switching protocols";
	_status_codes[102] = "Processing";
	_status_codes[103] = "Early Hints";

	//2xx Succesful
	_status_codes[200] = "OK";
	_status_codes[201] = "Created";
	_status_codes[202] = "Accepted";
	_status_codes[203] = "Non-Authoritative Information";
	_status_codes[204] = "No Content";
	_status_codes[205] = "Reset Content";
	_status_codes[206] = "Partial Content";
	_status_codes[207] = "Multi-Status";
	_status_codes[208] = "Already Reported";
	_status_codes[226] = "IM Used";

	//3xx Redirection
	_status_codes[300] = "Multiple Choices";
	_status_codes[301] = "Moved Permanently";
	_status_codes[302] = "Found (Previously \"Moved Temporarily\")";
	_status_codes[303] = "See Other";
	_status_codes[304] = "Not Modified";
	_status_codes[305] = "Use Proxy";
	_status_codes[306] = "Switch Proxy";
	_status_codes[307] = "Temporary Redirect";
	_status_codes[308] = "Permanent Redirect";

	//4xx Client Error
	_status_codes[400] = "Bad Request";
	_status_codes[401] = "Unauthorized";
	_status_codes[402] = "Payment Required";
	_status_codes[403] = "Forbidden";
	_status_codes[404] = "Not Found";
	_status_codes[405] = "Method Not Allowed";
	_status_codes[406] = "Not Acceptable";
	_status_codes[407] = "Proxy Authentication Required";
	_status_codes[408] = "Request Timeout";
	_status_codes[409] = "Conflict";
	_status_codes[410] = "Gone";
	_status_codes[411] = "Length Required";
	_status_codes[412] = "Precondition Failed";
	_status_codes[413] = "Payload Too Large";
	_status_codes[414] = "URI Too Long";
	_status_codes[415] = "Unsupported Media Type";
	_status_codes[416] = "Range Not Satisfiable";
	_status_codes[417] = "Expectation Failed";
	_status_codes[418] = "I'm a Teapot";
	_status_codes[421] = "Misdirected Request";
	_status_codes[422] = "Unprocessable Entity";
	_status_codes[423] = "Locked";
	_status_codes[424] = "Failed Dependency";
	_status_codes[425] = "Too Early";
	_status_codes[426] = "Upgrade Required";
	_status_codes[428] = "Precondition Required";
	_status_codes[429] = "Too Many Requests";
	_status_codes[431] = "Request Header Fields Too Large";
	_status_codes[451] = "Unavailable For Legal Reasons";

	//5xx Server Error
	_status_codes[500] = "Internal Server Error";
	_status_codes[501] = "Not Implemented";
	_status_codes[502] = "Bad Gateway";
	_status_codes[503] = "Service Unavailable";
	_status_codes[504] = "Gateway Timeout";
	_status_codes[505] = "HTTP Version Not Supported";
	_status_codes[506] = "Variant Also Negotiates";
	_status_codes[507] = "Insufficient Storage";
	_status_codes[508] = "Loop Detected";
	_status_codes[510] = "Not Extended";
	_status_codes[511] = "Network Authentication Required";
	return 0;
}


int Errors::setDefaultErrorResponse(int status)
{
    /* Error path is found from conf */
    int is_default_error = 1;
    error_response = "HTTP/1.1 ";
    error_response += std::to_string(status);
    error_response += " ";
    error_response += _status_codes.at(status);
    if(this->error_file_path.size())
    {
        this->error_file_path = concatStrings("src/", this->error_file_path);
        is_default_error = 0;
    }
    else
        this->error_file_path = "src/response/not_found.html";
    std::ifstream ifs(this->error_file_path);
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
				(std::istreambuf_iterator<char>()    ) );

    error_response += "\nContent-Type:text/html\nContent-Length: ";
    error_response +=  std::to_string(content.length());
    error_response += "\n\n";
    /* Change Error 404 from html to correct status */
    if(is_default_error){
        size_t found = content.find("Error 404");
        std::string status_str = "Error ";
        status_str += std::to_string(status);
        content.replace(found, status_str.length(), status_str);
    }
    error_response += content;

    return 0;
}

std::string Errors::getErrorResponse(){
    return this->error_response;
}

Errors::~Errors()
{
}

std::string Errors::getStatusText(int status_code)
{
	if (_status_codes.count(status_code))
	{
		return _status_codes[status_code];
	}
    return "";
}

std::string Errors::getStatusLine(int status_code)
{
	std::string res("HTTP/1.1 ");
	res += std::to_string(status_code) + " " + getStatusText(status_code) + "\n";
	return res;
}