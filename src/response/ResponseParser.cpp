#include "ResponseParser.hpp"
#include "server/Errors.hpp"
#include "cgi/Cgi.hpp"
#include "utils.hpp"
#include "types/MimeTypes.hpp"
#include "server/Errors.hpp"
#include "request/utils.hpp"

ResponseParser::ResponseParser(RequestParser req, std::map<int, Config *> configs_map)
{
    this->request = req;
    this->is_finish = 0;
    this->configs_map = configs_map;
    this->setCorrespondingLocation();
	std::cout << "serve root - " << serve_root << std::endl;
	this->launchResponse();
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
	http_methods.push_back("GET");
	http_methods.push_back("POST");
	http_methods.push_back("DELETE");
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
	Directives *loc = corresponding_location;
	MimeTypes mime_types;
	if (serve_root.find("favicon.ico") != std::string::npos)
	{
		if (!(is_file_exists(serve_root)))
		{
			return 0;
		}
	}

	if (!loc->_return.empty())
	{
		Errors err;
		this->_response = err.getStatusLine(302);
		this->_response += "Location: " + loc->getReturn() + "\n";
		this->_response += "Content-Length: 0\n\n";
		return 0;
	}

	if (!loc->getCgi().empty())
	{
		size_t pos = serve_root.rfind(".");
		std::string ext;
		if (pos != std::string::npos)
		{
			ext = serve_root.substr(pos + 1);
		}
		std::vector<std::pair<std::string, std::string> >::const_iterator it = loc->getCgi().begin();
		for (it = loc->getCgi().begin(); it != loc->getCgi().end(); ++it)
		{
			if (!ext.empty() && ext == it->first)
			{
				if (!is_file_exists(it->second))
				{
					throw (500);
				}
				if(!is_file_exists(serve_root))
					throw(404);
				int fd = Cgi::execute(*this, it->second);
				// checkCgi();
				std::string cgi_content;

				int		sz;
				char	buf[1024];

				while ((sz = read(fd, buf, 1023)) != 0)
				{
					buf[sz] = '\0';
					cgi_content += std::string(buf);
				}
				this->_response = generateResponseStringForString(200, cgi_content);
				close(fd);
				return 0;
			}
		}
	}

	std::string default_html = "<html><body><h1 style=\"color:blue;text-align:center\">Welcome to \"Webserv\" 42 project</body></html></h1>";
	if (is_file_exists(serve_root))
	{
		struct stat tmp_info;
		std::string concat_paths = serve_root;
		stat(concat_paths.c_str(), &tmp_info);
		int sz = tmp_info.st_size;
		if(is_regular_file(serve_root) || sz == 0)
		{
			this->_response = generateResponseStringForPath(200, serve_root);
			return 0;
		}
		else
		{
			if(loc->_index.empty())
			{
				
				if (loc->_autoindex == "on")
				{
					this->_response = generateResponseStringForString(200, getDirContentHTML(serve_root));
					return 0;
				}
				else
				{
					if (request.getRoute() == "/")
					{
						this->_response = generateResponseStringForString(200, default_html);
						return 0;
					}
					else
					{
						throw(404);
					}
				}
			}
			else
			{
				bool is_404 = true;
				for (unsigned long i = 0; i < loc->_index.size(); ++i)
				{				

					concat_paths = concatStrings(serve_root, loc->_index[i]);
					// this->request.removeMultipleForwardSlashes(concat_paths);
					if(is_file_exists(concat_paths) && (is_regular_file(concat_paths) || sz == 0))
					{
						this->_response = generateResponseStringForPath(200, concat_paths);
						is_404 = false;
						break;
					}
					if (is_404)
					{
						if (request.getRoute() == "/")
						{
							this->_response = generateResponseStringForString(200, default_html);
							return 0;
						}
						else
						{
							throw(404);
						}
					}
				}
			}
		}
	}
	else
	{
		if (request.getRoute() == "/")
		{
			this->_response = generateResponseStringForString(200, default_html);
			return 0;
		}
		else
		{
			throw(404);
		}
	}
    return 0;
}

int ResponseParser::checkMaxBodySize(){
	std::string max_body_size = this->corresponding_location->getClient_max_body_size();
	std::string suffix = "kKmM";
	if(max_body_size.size())
	{
		size_t pos = suffix.find(max_body_size[max_body_size.length() - 1]);
		if(pos != std::string::npos)
			max_body_size = max_body_size.substr(0, max_body_size.size()-1);
		if (is_number(max_body_size))
			this->max_body_size_bytes = atoi(max_body_size.c_str());
		else
			return 0;
		if((pos == 0) || (pos == 1))
			max_body_size_bytes *= 1000; 
		if ((pos == 2) || (pos == 3))
			max_body_size_bytes *= 1000000;
		std::cout << "Max body size - " << max_body_size_bytes << std::endl;
		if(max_body_size_bytes < this->request.getPostReqBody().size())
		{
			throw(413);
		}
	}
	return 0;
}

void ResponseParser::postWithoutCgi()
{
	MimeTypes types;
	std::string file_type = types.findInMapValue(this->request.getContentType());
	std::string filename;
	filename = this->request.getContentDisposition();
	if(filename.size())
	{
		size_t pos = filename.find("filename=");
		if (pos != std::string::npos) {
			filename = filename.substr(pos + 10); // 10 is the length of "filename="
			filename = filename.substr(1, filename.size() - 2); // Remove quotes
		}
	}
	else{
		filename = intToString<unsigned long long>(getCurrentTimeMilliseconds());
	}
	if(!file_type.size())
	{
		file_type = ".txt";
	}
	filename += file_type;
	std::string upl_path = this->corresponding_location->getUpload_path();
	if((upl_path.size()) && (upl_path[upl_path[upl_path.length() - 1]] != '/'))
		upl_path += '/';
	std::ofstream out(upl_path + filename);
	if(!(out.is_open()))
		throw(400);
	out << this->request.getPostReqBody();
	out.close();
}

int ResponseParser::generatePostResponse()
{
	checkMaxBodySize();
	checkUploadPath();
	if(this->request.getIsMultipart())
	{
		this->serve_root = "src/cgi/upload.py";
		int fd = Cgi::execute(*this, "");
		close(fd);
	}
	else if(this->request.getContentType().size())
	{
		this->postWithoutCgi();
	}
	else{
		throw(400);
	}
    this->_response ="HTTP/1.1 204 No Content\r\n"
            "Content-Length: 0\r\n"
            "\r\n";
    return 0;
}

int ResponseParser::generateDeleteResponse()
{
	if (std::remove(this->getServerRoot().c_str()) == -1) 
        throw(404);
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
	}
    return 0;
};

std::string ResponseParser::getContentLengthLine(const std::string &path)
{
	std::string res("Content-Length: ");
	struct stat filestatus;
	stat(path.c_str(), &filestatus );
	res += intToString<unsigned long long>(((unsigned long long)filestatus.st_size)) + "\n";
	return res;
}

std::string ResponseParser::generateResponseStringForPath(const int status_code, const std::string &path)
{
	Errors err;
	std::string resp = err.getStatusLine(status_code) + MimeTypes::getContentTypeLine(path) + getContentLengthLine(path) + "\n";
	std::ifstream ifs(path);
	std::string content( (std::istreambuf_iterator<char>(ifs) ),
						(std::istreambuf_iterator<char>()) );
	resp += content;
	ifs.close();
	return resp;
}

std::string ResponseParser::generateResponseStringForString(const int status_code, const std::string &content)
{
	Errors err;
	std::string resp = err.getStatusLine(status_code) + "Content-Type:text/html\nContent-Length: " + intToString<size_t>(content.length()) + "\n\n";
	resp += content;
	return resp;
}

int ResponseParser::checkUploadPath()
{
	struct stat s;
	std::string upload_path = this->corresponding_location->getUpload_path();
	std::cout << "upload_path " << upload_path << std::endl;
	if( stat(upload_path.c_str(), &s) == 0 )
	{
		if( s.st_mode & S_IFDIR )
		{
			std::cout << "Upload path was found\n";
			return 0;
		}
		else{
			throw(400);
		}
	}
	throw(400);
}

// bool ResponseParser::checkCgi() {
//     if (cgiPID != -1) {
//         // std::cout << ":checkCgi\n";
//         int status;
//         int waitRet;
//         waitRet = waitpid(cgiPID, &status, WNOHANG);
//         if (waitRet == -1) {
//             throw (500);
//         }
//         if (waitRet == 0 && time(NULL) - cgiStartTime > CGI_TIMEOUT) {
//             if (kill(cgiPID, SIGKILL) == -1) {
//                 throw std::runtime_error(std::string("kill: ") + strerror(errno));
//             };
//             waitpid(cgiPID, &status, 0);
//             cgiPID = -1;
//             if (WTERMSIG(status) == SIGKILL) {
//                 throw (508);
//             }
//             throw (500);
//         }
//         if (waitRet != 0 && WIFEXITED(status)) {
//             cgiPID = -1;
//             // std::ofstream osf("cgi_output.log");
//             // char buf[2000];
//             // buf[read(_cgiPipeFd, buf, 1999)] = '\0';
//             // osf << buf;
//             if (WEXITSTATUS(status) != 0) {
//                 // osf << this->getRequestBody();
//                 std::cout << "WEXITSTATUS(status) = " << WEXITSTATUS(status) << std::endl;
//                 throw (500);
//             }
//             std::cout << "WEXITSTATUS(status) = " << WEXITSTATUS(status) << std::endl;
//             // EvManager::addEvent(_cgiPipeFd, EvManager::read, EvManager::inner);
//             // this->addInnerFd(new InnerFd(_cgiPipeFd, *this, this->getResponseBody(), EvManager::read));
//         }
//     }
//     return (true);
// };