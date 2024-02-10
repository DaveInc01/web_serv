#ifndef IRESPONSEPARSER_HPP
#define IRESPONSEPARSER_HPP

#include "request/RequestParser.hpp"

class IResponseParser{
public:
    int             setFinish();
    Config* getCorresponding_server();
    int setCorrespondingLocation();
    Config *getMatchedServerName(std::vector<Config *>, int, std::string);
    /* Location which should be using for serveing data */
    Directives *corresponding_location;
    std::map<int, Config *> configs_map;
    RequestParser   request;
protected:
    std::string     buffer;
    std::string     content_type;
    std::string     server_name;
    std::string     date;
    std::string     response_body;
    int             content_length;
    int             is_finish;
    int             status_code;
    int             http_version;
};

int findPosition(const std::vector<int>& vec);
int longestCommonPrefix(const std::string& str, const std::vector<std::string>& vec);
int findIndexOfMax(const std::vector<int>& vec);

#endif