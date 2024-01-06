#include "ResponseParser.hpp"

ResponseParser::ResponseParser(){}

ResponseParser::ResponseParser(RequestParser req){
    this->request = req;
    this->is_finish = 0;
}

int ResponseParser::launchResponse()
{
    
    return 0;
};

