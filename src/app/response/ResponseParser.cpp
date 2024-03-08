#include "ResponseParser.hpp"
#include "server/Errors.hpp"

ResponseParser::ResponseParser(RequestParser req, std::map<int, Config *> configs_map)
{
    this->request = req;
    this->is_finish = 0;
    this->configs_map = configs_map;
    this->setCorrespondingLocation();
	this->launchResponse();
	// this->setErrorMap();
}

void ResponseParser::setResponse(std::string r){
	this->_response = r;
}

std::string ResponseParser::checkErrorFromConf(int status)
{
	std::vector<std::pair<std::string, std::string> > error_from_conf = this->corresponding_location->getError_page();

	if(error_from_conf.empty())
	{
		return NULL;
	}
	else{
		for(std::vector<std::pair<std::string, std::string> >::iterator it = error_from_conf.begin(); it != error_from_conf.end(); ++it)
		{
			if(atoi(it->first.c_str()) == status)
			{
				return it->second;
			}
		}
	}
	return NULL;
}

int ResponseParser::checkIsAloowedMethod(){
	std::string req_method = this->request.getMethod();
	std::vector<std::string> http_methods;
	http_methods.insert(http_methods.end(), {"GET", "POST", "DELETE"});
	std::vector<std::string>::iterator found = std::find(http_methods.begin(), http_methods.end(), req_method);
	if(found == http_methods.end())
		throw(405);
	std::vector<std::string> allowed_methods = this->corresponding_location->getMethods();
	found = std::find(allowed_methods.begin(), allowed_methods.end(), req_method);
	if(found == allowed_methods.end())
		throw(405);
	return 0;
}	

int ResponseParser::generateGetResponse()
{
    return 0;
}

int ResponseParser::generatePostResponse()
{
	std::cout << this->request.getContentType() << std::endl;
    std::ofstream out("output.jpg");
	out << this->request.getPostReqBody();
	out.close();
    return 0;
}

int ResponseParser::generateDeleteResponse()
{
    return 0;
}


int ResponseParser::launchResponse()
{
	std::string req_method = this->request.getMethod();
	try{
		this->checkIsAloowedMethod();
		if(req_method == "GET")
		{
			this->generateGetResponse();
		}
		else if(req_method == "POST")
		{
			this->generatePostResponse();
		}
		else if(req_method == "DELETE")
		{
			this->generateDeleteResponse();
		}
	}
	catch(int status)
	{
		
		Errors *e = new Errors(status, *this);
		this->_response = e->getErrorResponse();
		delete e;
		return 0;
		
		// std::cout << "The response is returned staus code ---- " << status << std::endl;
	}
	std::string arr="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
	std::string my_response = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: ";
	struct stat filestatus;
	stat("www/index.html", &filestatus );
	// std::cout << this->request.getHttpReq();
	my_response += std::to_string(filestatus.st_size) + "\n\n";
	std::ifstream ifs("www/index.html");
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
				(std::istreambuf_iterator<char>()    ) );
	
	my_response	+= content;
	this->_response = my_response;
	ifs.close();
    return 0;
};

