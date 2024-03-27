#ifndef ALLCONFIGS_HPP
#define ALLCONFIGS_HPP

#include "Config.hpp"

class AllConfigs
{
public:
    int _servsCount;
    std::map<int, Config *> _AllServs;
    std::vector<std::string> _directiv_list;
    std::string _st;

public:
    AllConfigs();
    ~AllConfigs();
    
    void readConff();
    size_t find_server_end(std::string str);
    void make_location(std::string &s, Config &S);
    Config *makeServer(const std::string &file);

    Config const&  get_Server(int n) const;
    Directives const* get_location(int n, std::string str) const;
    void chech_directive();

    //CHeck

    void    dir_is_valid(std::string dir, int from);
    void    check_validity(std::string const &full) const;
    void    check_parentheses(std::string const &full) const;
    void    cut_location(std::string &s, Config *S);
    
    void clearSpaces(std::string line);
    static bool isSpace(char a, char b);
};

#endif