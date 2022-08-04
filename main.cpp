#include <iostream>
#include <vector>
#include "docker.hpp"
#include "ssh_server.hpp"
#include "connection.hpp"

int main()
{
    std::vector<char*> commands ;
    commands.push_back("ls");
    commands.push_back("ls -a");
    commands.push_back("docker ps");
    commands.push_back("docker system prune -f");
    SSHConnection* conn = new SSHConnection(commands);
    std::cout << "Created connection succesfully" << std::endl;
    conn->connectAndRun("www.zacwaite.xyz", "22", "zac", "");
    delete conn;
}
