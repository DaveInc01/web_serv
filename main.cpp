#include "server/Server.hpp"

int main(){
    AllConfigs configs;
    try{
        configs.readConff();
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;;
        return (0);
    }
    catch(...)
    {
        std::cout << "Invalid config file" << std::endl;
        return (0);
    }
    try{
        Server server1(configs._AllServs);
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
