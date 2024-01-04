#include "ResponseParser.hpp"

ResponseParser::ResponseParser(){};

ResponseParser::ResponseParser(RequestParser req){
    this->request = req;
}
