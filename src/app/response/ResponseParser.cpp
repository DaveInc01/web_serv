#include "ResponseParser.hpp"

ResponseParser::ResponseParser(){}

ResponseParser::ResponseParser(RequestParser req, std::map<int, Config *> configs_map)
{
    this->request = req;
    this->is_finish = 0;
    this->configs_map = configs_map;
    this->setCorrespondingLocation();
	this->launchResponse();
}

int ResponseParser::launchResponse()
{
	std::string arr="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
	std::string my_response = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: ";
	struct stat filestatus;
	stat("src/www/index.html", &filestatus );

	std::cout << this->request.getHttpReq();

	my_response += std::to_string(filestatus.st_size) + "\n\n";
	// int status = system("open src/www/index.html");
	std::ifstream ifs("src/www/index.html");
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
				(std::istreambuf_iterator<char>()    ) );
	
	my_response+= content;
	this->_response = arr;
    return 0;
};

