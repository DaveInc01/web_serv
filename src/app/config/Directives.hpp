#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <sstream>


typedef std::vector<std::pair<std::string,  std::vector<std::string> > > Vec;

class Directives 
{
public:
    Vec directives;
public:
    Directives();
    Directives(Vec const &d_s);
    // Directives(Vec const &&d_s) {
    //     std::cout << "\n\n\n\n\nDirectives(Vec const &&d_s)\n\n\n\n\n\n";
    // };
    ~Directives();
    
    void add_directives(std::pair<std::string,  std::vector<std::string> > &p);
    Vec const & get_directives() const;

};







#endif