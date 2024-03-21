#pragma once

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include "response/ResponseParser.hpp"

#ifdef __linux__
# define PHP_CGI_PATH "src/cgi/cgi_interpreters/php-cgi-linux"
# define PYTHON_CGI_PATH "src/cgi/cgi_interpreters/python-cgi-linux"
#else
# define PHP_CGI_PATH "src/cgi/cgi_interpreters/php-cgi-mac"
# define PYTHON_CGI_PATH "src/cgi/cgi_interpreters/python-cgi-mac"
#endif

#define CGI_TIMEOUT 120 

class ResponseParser;

class Cgi
{
    public:
		static int execute(ResponseParser &client, const std::string &cgi_path = "");
        static char **initEnv(ResponseParser &client);
    private:
        Cgi();
        Cgi(const Cgi &rhs);
        Cgi &operator=(const Cgi &rhs);
        ~Cgi();
    private:
        static std::map<std::string, std::string> _env;
};