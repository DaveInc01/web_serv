#include "cgi/Cgi.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <ctime>

std::map<std::string, std::string> Cgi::_env;

int Cgi::execute(ResponseParser &client) {
    signal(SIGPIPE, SIG_IGN); // TODO
    char *argv[3];
    // const std::string &argv1 = client.getCurrentLoc().getCgi(client.getExtension()).second;
    const std::string &argv1 = "src/cgi/cgi_interpreters/python-cgi-mac";
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
    std::ofstream osf("log.log");
    osf << client.request.getPostReqBody();
    osf.flush();
    osf.close();
    if (pid == 0) {
        if (client.request.getMethod() == "POST") {
            int fd = open("log.log", O_RDWR);
            std::cout << "fd = " << fd << std::endl;
            dup2(fd, 0);
            close(fd);
        }
        char **envp = Cgi::initEnv(client);

        dup2(pipe_from_child[1], 1);
        close(pipe_from_child[0]);
        close(pipe_from_child[1]);
        int res = execve(argv[0], argv, envp);
        perror("execve: ");
        // exit(res);
        exit(1);
    }
    waitpid(pid, NULL, 30000);
    close(pipe_from_child[1]);
    // client.setCgiPID(pid);
    // client.setCgiStartTime();
    return (pipe_from_child[0]);
};

char **Cgi::initEnv(ResponseParser &client)
{
    char *pwd;
    // const ServerCore &srv = client.getCurrentLoc();
    // client.showHeaders();
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
    _env["SERVER_WRITE_PATH"] = client.corresponding_location->getUpload_path();
    _env["UPLOAD_DIR"] = client.corresponding_location->getUpload_path();
    _env["LC_CTYPE"] = "C.UTF-8";
    _env["REDIRECT_STATUS"] = "true";
	free(pwd);

    char **envp = new char *[_env.size() + 1];

	int i = 0;
    std::ofstream ofs("env.log");
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); ++it)
	{
		envp[i++] = strdup((it->first + "=" + it->second).c_str());
        ofs << envp[i - 1] << std::endl;;
	}

	envp[i] = NULL;
    // delete envp;
	return envp;
};