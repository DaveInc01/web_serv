#include "ResponseParser.hpp"

ResponseParser::ResponseParser(){}

ResponseParser::ResponseParser(RequestParser req){
    this->request = req;
    this->is_finish = 0;
    this->setCorrespondingLocation();
}

int ResponseParser::launchResponse()
{
    return 0;
};

int ResponseParser::setCorrespondingLocation(){
    std::cout << "The host name is - " << this->request.getHost() << std::endl;
    return 0;
}


