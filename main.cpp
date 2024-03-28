#include "server/Server.hpp"

int main(int ac, char **av){
    if (ac > 2)
    {
        std::cout << "invalid: many parametr av" << std::endl;
        return (0);
    }    
    AllConfigs configs;
    try{
        if(ac == 2)
            configs._st = av[1];
        configs.readConff();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;;
        return (0);
    }
    catch(...)
    {
        std::cout << "Some Error has ocured" << std::endl;
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
        std::cout << "Some Error has ocured" << std::endl;
    }
    return 0;
}