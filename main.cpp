#include <vector>
#include <iostream>
#include "ssh_server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 7) {
        std::cout << "Usage: " << argv[0] << " <image_name> <port> <ssh_host> <ssh_port> <ssh_user> <ssh_password>" << std::endl;
        return 1;
    }
    return runDeploymentCommands("ubuntu", "8004", "www.zacwaite.xyz", "22", "zac", "");
}
