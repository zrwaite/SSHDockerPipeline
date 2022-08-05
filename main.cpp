#include <vector>
#include <iostream>
#include "ssh_server.hpp"
#include "validator.hpp"

int main(int argc, char *argv[])
{
    if (argc != 7) {
        std::cout << "Usage: " << argv[0] << " <image_name> <port> <ssh_host> <ssh_port> <ssh_user> <ssh_password>" << std::endl;
        std::cout << "You provided " << argc-1 << " args";
        return 1;
    }
    if (!validateImageName(argv[1])) {
        std::cout << "Invalid image name: " << argv[1] << std::endl;
        return 1;
    }
    if (!validatePort(argv[2])) {
        std::cout << "Invalid port: " << argv[2] << std::endl;
        return 1;
    }
    if (!validatePort(argv[4])) {
        std::cout << "Invalid ssh_port: " << argv[4] << std::endl;
        return 1;
    }

    return runDeploymentCommands(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
}
