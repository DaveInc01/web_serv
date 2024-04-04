#include "AllConfigs.hpp"
#include <cctype>
#include <algorithm>
AllConfigs::AllConfigs()
{
    const char* directiv_list[] = {"server_name", "listen", "root", "index", "autoindex", "error_page", "client_max_body_size", "cgi", "allow_methods", "return", "upload_path", "methods"};
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
    Directives *m = new Directives(S);
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
    for(int i = 0; i < (int)fv.size(); ++i)
    {
        std::stringstream ss(fv[i]);
        while(ss >> key)
        {
            if(key == "location")
            {
                ss >> mkey; // to pass from "location" to mkey
                size_t pos = mkey.find('{');
                if (pos != std::string::npos) {
                    tok = mkey[pos];
                    mkey.erase(pos, 1);
                }
                else {
                    ss >> tok; // to pass from "mkey" to "{"
                }
                size_t tok_pos = tok.find('{');
                if (tok.length() != 1 && tok_pos != std::string::npos) {
                    tok.erase(tok_pos, 1);
                    key = tok;
                }
                else
                    ss >> key; // to pass from "{" to next word key
            }
            
            value.clear();
            while(ss >> tok)
            {
                if(std::find(_directiv_list.begin(), _directiv_list.end(), tok) != _directiv_list.end())
                    throw std::invalid_argument("Invalid syntsx:local_interval");
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
    std::pair<std::string, Directives *> p;
    if(mkey.find_last_of('/') == mkey.size()-1 && mkey.size() != 1)
    {
        p.first = mkey.substr(0,mkey.size()-1);
    }
    else
        p.first = mkey;
    p.second = m;
    S.add_locations(p);
}


void AllConfigs::cut_location(std::string &s, Config *S) {
    size_t ilocation = s.find("location");
    if (s[ilocation - 2] != ';' && ilocation != std::string::npos)
        throw std::invalid_argument("Syntax error");

    while(ilocation != std::string::npos)
    {
        size_t i_end_location = s.find("}");
        std::string str = s.substr(ilocation, i_end_location - ilocation + 1);
        make_location(str, *S);
        s.erase(ilocation, i_end_location - ilocation + 1);
        ilocation = s.find("location");
    }
}

Config *AllConfigs::makeServer(const std::string &file)
{
    std::string s = file;
    Config *S = new Config;

    cut_location(s, S);
    std::stringstream ss(s);
    
    std::string tok;
    std::string key;
    std::vector <std::string> value;
    std::vector<std::string> fv;

    while(getline(ss, tok, ';'))
    {
        fv.push_back(tok);
    }
    for(int i = 0; i < (int)fv.size(); ++i)
    {
        std::stringstream ss(fv[i]);
        while(ss >> key)
        {
            size_t pos = key.find("{");
            if (pos != std::string::npos)
            {
                key.erase(pos, 1);
                ss >> key;
            }
            else if(key == "server")
            {
                ss >> key; // to pass from "server" to "{"
                size_t key_pos = key.find("{");
                if (key.length() != 1 && key.find("{") != std::string::npos) {
                    key.erase(key_pos, 1);
                }
                else
                    ss >> key; // to pass from "{" to next word key
            }
            

            value.clear();
            while(ss >> tok)
            {                
                if(std::find(_directiv_list.begin(), _directiv_list.end(), tok) != _directiv_list.end())
                    throw std::invalid_argument("Invalid syntsx:");
                value.push_back(tok);
            }
            if(key != "}")
            {
                dir_is_valid(key, 0);
                std::pair<std::string,  std::vector<std::string> > p;
                p.first = key;
                p.second = value;
                if (key == "listen")
                    S->add_listen(p);
                else if (key == "server_name")
                    S->add_servername(p);
                else
                    S->add_directives(p);
            }
        }
    }

    S->fillLocations();
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

void AllConfigs::clearSpaces(std::string line) {
    line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
    line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1, line.length());
}

bool AllConfigs::isSpace(char a, char b) {
    return (isspace(a) && isspace(b));
}

void AllConfigs::readConff()
{
    std::ifstream fin;
    if (_st.empty())
        fin.open("src/config/config.conf");
    else
    {
        fin.open(_st);
    }
    if (!fin.is_open())
    {
        throw std::invalid_argument("Configuration file not found");
    }
    std::string line;
    std::string full;
    std::string new_line;
    fin >> line;
    full = line + " ";
    std::string line_2;
    if(line != "server" && line != "server{")
    {
        throw std::invalid_argument("Not server");
    }   ///

    // while(std::getline(fin, line))
    while(fin >> line)
    {
        full += " " + line;
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

void    AllConfigs::check_validity(std::string const &full)const
{
    check_parentheses(full);
}

void    AllConfigs::check_parentheses(std::string const &full)const
{
    int p = 0;
    for(int i = 0; i < (int)full.size(); ++i)
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

Config const& AllConfigs::get_Server(int n) const
{
    return (*(_AllServs.at(n)));
}

Directives const* AllConfigs::get_location(int n, std::string str) const
{
    std::vector<std::pair<std::string, Directives *> > p = (_AllServs.at(n)->get_locations());
    std::vector<std::pair<std::string, Directives *> >::iterator itb = p.begin();
    std::vector<std::pair<std::string, Directives *> >::iterator ite = p.end();
    for(; itb != ite; ++itb)
    {
        if(itb->first == str)
        {
            return (itb->second);
        }
    }
    return (NULL);
}

void AllConfigs::chech_directive()
{
    Config const &s = get_Server(1);
    Directives const *l = get_location(1, "/");
    s.printConfig();
    if (l != NULL)
        l->printDirective();
}