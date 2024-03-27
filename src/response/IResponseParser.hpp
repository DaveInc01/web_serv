#ifndef IRESPONSEPARSER_HPP
#define IRESPONSEPARSER_HPP

#include "request/RequestParser.hpp"
// #include "cgi/Cgi.hpp"
// #include "server/Server.hpp"
class IResponseParser{
public:
    int             setFinish();
    Config*         getCorrespondingServer();
    int             setCorrespondingLocation();
    Directives      *getCorrespondingLocation(Config*);
    Config          *getMatchedServerName(std::vector<Config *>, std::string);
    int             checkDefaultLocation(Config*);
    int             findInVect(std::string, Config*);
    int             setServeRoot();
	int				cutResponse(int);
    void            setCgiPId(int);
    void            setCgiStartTime();
public:
    /* Location which should be using for serveing data */
    Directives      *corresponding_location;
    RequestParser   request;
    std::map<int, Config *> configs_map;
    std::string getServerRoot();
protected:
    std::string     location_class_path;
    std::string     serve_root;
    std::string     buffer;
    std::string     content_type;
    std::string     server_name;
    std::string     date;
    std::string     response_body;
    std::string     boundary;
    std::string     multipart_body;
    int             content_length;
    int             is_finish;
    int             status_code;
    int             http_version;
    int             have_def_location;
	unsigned long int   max_body_size_bytes;
    int            cgiPID;
    std::time_t    cgiStartTime;
};

int findPosition(const std::vector<int>& vec);
int longestCommonPrefix(const std::string& str, const std::vector<std::string>& vec);
int findIndexOfMax(const std::vector<int>& vec);

#endif