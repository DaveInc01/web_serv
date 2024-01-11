#include "AllConfigs.hpp"

AllConfigs::AllConfigs()
{
    const char* directiv_list[] = {"server_name", "listen", "root", "index", "autoindex", "error_page", "client_max_body_size", "cgi", "allow_methods", "upload_dir", "return", "upload_path", "methods"};
    const int numDirs = sizeof(directiv_list) / sizeof(directiv_list[0]);

    for (int i = 0; i < numDirs; ++i) {
        _directiv_list.push_back(directiv_list[i]);
    }
    _servsCount = 0;
}

AllConfigs::~AllConfigs() {
    std::map<int, Config *>::iterator it = _AllServs.begin();
    while(it != _AllServs.end())
    {
        delete it->second;
        *it++;
    }
};

size_t AllConfigs::find_server_end(std::string str)
{
    size_t sz = str.size();
    size_t posible_end = str.find("}");
       
    while(posible_end != sz - 1 && str[posible_end + 2] != 's')
    {
        posible_end = str.find_first_of("}",  posible_end + 1);
    }

    return posible_end;
}

void AllConfigs::make_location(std::string &s, Config &S)
{
    Directives *m = new Directives;
    std::string tok;
    std::string key;
    std::string mkey;
    std::vector <std::string> value;
    std::vector<std::string> fv;
    std::stringstream ss(s);

    while(getline(ss, tok, ';'))
    {
        fv.push_back(tok);
    }
    for(int i = 0; i < fv.size(); ++i)
    {
        std::stringstream ss(fv[i]);
        while(ss >> key)
        {
            if(key == "location")
            {
                ss >> mkey; // to pass from "location" to mkey 
                ss >> tok; // to pass from "mkey" to "{"
                ss >> key; // to pass from "{" to next word key
            }
            
            value.clear();
            while(ss >> tok)
            {
                value.push_back(tok);
            }
            if(key != "}")
            {
                dir_is_valid(key, 2);

                std::pair<std::string,  std::vector<std::string> > p;
                p.first = key;
                p.second = value;
                m->add_directives(p);
            }
               
        }
    }
    std::pair<std::string, Directives> p;
    p.first = mkey;
    p.second = *m;
    S.add_locations(p);
}

Config *AllConfigs::makeServer(std::string &s)
{

    Config *S = new Config;

    size_t ilocation = s.find("location");
    while(ilocation != std::string::npos)
    {
        size_t i_end_location = s.find("}");
        std::string str = s.substr(ilocation, i_end_location - ilocation + 1);
        make_location(str, *S);
        s.erase(ilocation, i_end_location - ilocation + 1);
        ilocation = s.find("location");
    }


    std::stringstream ss(s);
    int servisopen = 0;
    int lockisopen = 0;
    std::string tok;
    std::string key;
    std::vector <std::string> value;
    std::vector<std::string> fv;


    while(getline(ss, tok, ';'))
    {
        fv.push_back(tok);
    }
    for(int i = 0; i < fv.size(); ++i)
    {
        std::stringstream ss(fv[i]);
        while(ss >> key)
        {
            if(key == "server")
            {
                ss >> key; // to pass from "server" to "{" 
                ss >> key; // to pass from "{" to next word key
            }
            
            value.clear();
            while(ss >> tok)
            {
                value.push_back(tok);
            }
            if(key != "}")
            {
                dir_is_valid(key, 0);
                std::pair<std::string,  std::vector<std::string> > p;
                p.first = key;
                p.second = value;
                S->add_directives(p);
            }
        }
    }
    return S;
            
}

void AllConfigs::dir_is_valid(std::string dir, int from)
{
    std::vector<std::string>::iterator it = std::find(next(_directiv_list.begin(), from), _directiv_list.end(), dir);
    if(it == _directiv_list.end())
    {
        throw std::invalid_argument("invalid directiv");
    }

}
void AllConfigs::readConff()
{
    std::ifstream fin("config2.conf");
    std::string line;
    std::string full;
    std::string path;
    std::vector<std::string> fv;
    fin >> line;  ///
    full = line + " ";
    if(line != "server")
    {
        throw std::invalid_argument("Not server");
    }   ///
    while(fin >> line)
    {
        full = full + " " + line;
    }

    check_validity(full);

    while(full.size() > 0)
    {
        
        _servsCount++;
        size_t serv_end = find_server_end(full);
        std::string serv = full.substr(0, serv_end + 1);
        _AllServs.insert(std::pair<int, Config *>(_servsCount, makeServer(serv)));
        full.erase(0, serv_end + 1);
    }   
}


void AllConfigs::print_vect(std::vector<std::string> &v)
{
    for(int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;

}


void AllConfigs::print_map(Vec m)
{
    std::cout << " LOCATION SIZE " << m.size() << " \n";
    Vec::iterator it = m.begin();
    for(; it != m.end(); ++it)
    {
        std::cout << "  " << it->first << " - ";
        print_vect(it->second);
    }
}



void AllConfigs::print_serv(int n)
{
    Vec m = _AllServs[n]->get_directives();
    Vec::iterator it = m.begin();
    std::cout << "server size " << m.size() << std::endl;
    for(; it != m.end(); ++it)
    {
        std::cout << "  " << it->first << " - ";
        print_vect(it->second);
    }
    std::vector<std::pair<std::string, Directives> > loc = _AllServs[n]->get_locations();
    std::vector<std::pair<std::string, Directives> >::iterator i = loc.begin();
    for(; i != loc.end(); ++i)
    {
        std::cout << "Locationnn  " << i->first << std::endl;
        Directives d = i->second;
        Vec m = d.get_directives();
        print_map(m);
    }
}

void AllConfigs::print_AllServs()
{
    for(int i = 0; i < _servsCount; ++i)
    {
        std::cout << "Server N - " << i+1 <<" is :\n";
        print_serv(i + 1);
    }
}


void    AllConfigs::check_validity(std::string const &full)const
{
    check_parentheses(full);
}

void    AllConfigs::check_parentheses(std::string const &full)const
{
    int p = 0;
    for(int i = 0; i < full.size(); ++i)
    {
        if(full[i] == '{')
        {
            p++;
        }
        else if(full[i] == '}')
        {
            p--;
            if(p < 0)
            {
                throw std::invalid_argument("\nInvalid parentheses 1 !!!\n");
            }
        }
    }
    if(p != 0)
    {
        throw std::invalid_argument("\nInvalid parentheses!!!\n");
    }
}
