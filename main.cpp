#include "server/Server.hpp"

int main(){
    Server server1;
    try{
        server1.launchConfig();
    }
    catch(const std::exception& e)
    {
        // std::cout << "hhhhh" << std::endl;
        std::cerr << e.what() << std::endl;;
        // return (0);
    }
    catch(...)
    {
        std::cout << "hhhhh" << std::endl;
        // return (0);
        // std::cerr << e.what() << std::endl;;
    }
    try{
        server1.launchServer();
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
    }
     catch(...)
    {
        std::cout << "Some error" << std::endl;
    }
    return 0;
}
