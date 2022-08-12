#include <vector>
#include <iostream>
#include "ssh_server.hpp"
#include "validator.hpp"

int main(int argc, char *argv[])
{
    if (argc != 9) {
        std::cout << "Usage: " << argv[0] << "<dockerHubAccessToken> <dockerUser> <image_name> <port> <ssh_host> <ssh_port> <ssh_user> <ssh_password>" << std::endl;
        std::cout << "You provided " << argc-1 << " args";
        return 1;
    }
    if (!validateImageName(argv[3])) {
        std::cout << "Invalid image name: " << argv[3] << std::endl;
        return 1;
    }
    if (!validatePort(argv[4])) {
        std::cout << "Invalid port: " << argv[4] << std::endl;
        return 1;
    }
    if (!validatePort(argv[6])) {
        std::cout << "Invalid ssh_port: " << argv[6] << std::endl;
        return 1;
    }

    return runDeploymentCommands(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
}
