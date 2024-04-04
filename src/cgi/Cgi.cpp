#include "cgi/Cgi.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <ctime>

std::map<std::string, std::string> Cgi::_env;

int Cgi::execute(ResponseParser &client, const std::string &cgi_path) {
    signal(SIGPIPE, SIG_IGN); // TODO
    char *argv[3];
    // const std::string &argv1 = client.getCurrentLoc().getCgi(client.getExtension()).second;
    // const std::string &argv1 = PYTHON_CGI_PATH;
	const std::string &argv1 = cgi_path.empty() ? PYTHON_CGI_PATH : cgi_path;
    argv[0] = const_cast<char *>(argv1.c_str());
    const std::string &argv2 =  client.getServerRoot();
    argv[1] = const_cast<char *>(argv2.c_str());
    argv[2] = NULL;
    int pipe_from_child[2];

    if (pipe(pipe_from_child) == -1
            || fcntl(pipe_from_child[0], F_SETFL, O_NONBLOCK, O_CLOEXEC) == -1) {
        throw (500);
    }
    int pid = fork();

    if (pid == -1) {
        throw (500);
    }
	std::string tmp_file_name = intToString<int>(client.request.getFd()) + "_log.log";
    std::ofstream osf(tmp_file_name);
    osf << client.request.getPostReqBody();
    osf.flush();
    osf.close();
    if (pid == 0) {
    alarm(1);
        if (client.request.getMethod() == "POST") {
            int fd = open(tmp_file_name.c_str(), O_RDWR);
            dup2(fd, 0);
            close(fd);
        }
        char **envp = Cgi::initEnv(client);
        dup2(pipe_from_child[1], 1);
        close(pipe_from_child[0]);
        close(pipe_from_child[1]);
        if(execve(argv[0], argv, envp) == -1)
        {
            for(unsigned int i = 0; envp[i]!= NULL; i++){
                delete envp[i];
            }
            delete envp;
            
            std::cerr << "Cgi Error\n";
            throw(501);
        }
        exit(1);
    }
    int status;
    waitpid(pid, &status, 0); // Wait for the child to finish
    std::remove(tmp_file_name.c_str());
    close(pipe_from_child[1]);
    return (pipe_from_child[0]);
};

char **Cgi::initEnv(ResponseParser &client)
{
    char *pwd;
    pwd = getcwd(NULL, 0);
    _env["AUTH_TYPE"] = "Basic";
    std::string length = client.request.getContentLength();
    if (length.empty() == false) {
        _env["CONTENT_LENGTH"] = length;
    }
    _env["CONTENT_TYPE"] = client.request.getContentType();
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = client.getServerRoot();
    _env["PATH_TRANSLATED"] = pwd + std::string("/") + client.getServerRoot();
    _env["QUERY_STRING"] = client.request.getQueryString();
    _env["REMOTE_ADDR"] = client.request.getClientIp();
    _env["REMOTE_HOST"] = client.request.getHost();
    _env["REMOTE_USER"] = pwd;
    _env["REQUEST_METHOD"] = client.request.getMethod();
    _env["SCRIPT_NAME"] = client.getServerRoot();
    _env["SCRIPT_FILENAME"] = pwd + std::string("/") + client.getServerRoot();
    _env["SERVER_NAME"] = "web_serv";
    _env["SERVER_PORT"] = client.request.getPort_str();
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["SERVER_SOFTWARE"] = "Web_serv";
	std::string upl_path = client.corresponding_location->getUpload_path();
	if((upl_path.size()) && (upl_path[upl_path[upl_path.length() - 1]] != '/'))
		upl_path += '/';
    _env["SERVER_WRITE_PATH"] = upl_path;
    _env["UPLOAD_DIR"] = client.corresponding_location->getUpload_path();
    _env["LC_CTYPE"] = "C.UTF-8";
    _env["REDIRECT_STATUS"] = "true";
	free(pwd);

    char **envp = new char* [_env.size() + 1];

	int i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it)
	{
        std::string elem = it->first + "=" + it->second;
        char *cstr = new char[elem.size() + 1];
        std::strcpy(cstr, elem.c_str());
		envp[i++] = cstr;
        // envp[i++] = strdup((it->first + "=" + it->second).c_str());
	}
	envp[i] = NULL;
	return envp;
};