#include "server/Server.hpp"

int main(){
    try{
        Server::launchConfig();
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
    Server::launchServer();

    return 0;
}
