#include "IRequestParser.hpp"

std::string& rtrim(std::string& s)
{
	const char* ws = " \t\n\r\f\v";
    s.erase(s.find_last_not_of(ws) + 1);
    return s;
}

std::string& ltrim(std::string& s)
{
	const char* ws = " \t\n\r\f\v";
    s.erase(0, s.find_first_not_of(ws));
    return s;
}

std::pair<std::string, std::string> ft_split(std::string s, char del){
    std::pair<std::string, std::string> arr;
    std::string left, right;
    int i = s.find(del);
    left = s.substr(0, i);
    right = s.substr(i+1, s.size());
    arr.first = rtrim(left);
    arr.second = ltrim(rtrim((right)));
    return (arr);
}