#include <vector>
#include <iostream>
#include "ssh_server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 7) {
        std::cout << "Usage: " << argv[0] << " <image_name> <port> <ssh_host> <ssh_port> <ssh_user> <ssh_password>" << std::endl;
        return 1;
    }
    return runDeploymentCommands("zrwaite/codegraphs:3.8", "8001", "www.zacwaite.xyz", "22", "zac", "");
}
