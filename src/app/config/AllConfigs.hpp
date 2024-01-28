#ifndef ALLCONFIGS_HPP
#define ALLCONFIGS_HPP

#include "Config.hpp"

class AllConfigs
{
public:
    int _servsCount;
    std::map<int, Config *> _AllServs;
    std::vector<std::string> _directiv_list;

public:
    AllConfigs();
    ~AllConfigs();
    
    void readConff();
    size_t find_server_end(std::string str);
    void make_location(std::string &s, Config &S);
    Config *makeServer(std::string &s);
    void print_AllServs();
    void print_serv(int n);
    void print_vect(std::vector<std::string> &v);
    void print_map(Vec);

    //CHeck

    void    dir_is_valid(std::string dir, int from);
    void    check_validity(std::string const &full) const;
    void    check_parentheses(std::string const &full) const;

};


#endif