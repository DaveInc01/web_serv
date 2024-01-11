#include "Directives.hpp"


Directives::Directives(Vec const &d_s) : directives(d_s)
{
    
}
Directives::Directives()
{
    
}
Directives::~Directives()
{
    
}

Vec const & Directives::get_directives() const
{   
    return (directives);
}

void Directives::add_directives(std::pair<std::string,  std::vector<std::string> > &p)
{
    directives.push_back(p);
}
