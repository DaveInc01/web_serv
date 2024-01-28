#include "ResponseParser.hpp"

ResponseParser::ResponseParser(){}

ResponseParser::ResponseParser(RequestParser req, std::map<int, Config *> configs_map)
{
    this->request = req;
    this->is_finish = 0;
    this->configs_map = configs_map;
    this->setCorrespondingLocation();
}

int ResponseParser::launchResponse()
{
    return 0;
};

