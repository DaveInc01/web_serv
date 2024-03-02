#include "server/Errors.hpp" 

int Errors::setErrorMap(){
	_errors[100] = "Continue";
	_errors[200] = "OK";
	_errors[201] = "Created";
	_errors[204] = "No Content";
	_errors[400] = "Bad Request";
	_errors[403] = "Forbidden";
	_errors[404] = "Not Found";
	_errors[405] = "Method Not Allowed";
	_errors[413] = "Payload Too Large";
	_errors[500] = "Internal Server Error";
	return 0;
}

int Errors::setDefaultErrorResponse(int status)
{
    _def_error_response = "HTTP/1.1 ";
    _def_error_response += std::to_string(status);
    _def_error_response += " ";
    _def_error_response += _errors.at(status);
    std::ifstream ifs("src/response/not_found.html");
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
				(std::istreambuf_iterator<char>()    ) );

    _def_error_response += "\nContent-Type:text/html\nContent-Length: ";
    _def_error_response +=  std::to_string(content.length());
    _def_error_response += "\n\n";
    _def_error_response += content;
    return 0;
}

Errors::Errors(int status, std::vector<std::pair<std::string, std::string> > conf_error)
{
    setErrorMap();
    this->setDefaultErrorResponse(status);
}

std::string Errors::getErrorResponse(){
    return this->_def_error_response;
}

Errors::~Errors()
{
}