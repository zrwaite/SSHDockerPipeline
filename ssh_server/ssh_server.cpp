#include <vector>
#include <string>
#include "docker.hpp"
#include "ssh_server.hpp"
#include "connection.hpp"

int runDeploymentCommands(char* imageName, char* port, char* sshHost, char* sshPort, char* sshUser, char* sshPassword) {
	std::string imageNameString(imageName);
	const char* versionLessImage = removeVersion(imageNameString).c_str();
	std::vector<const char*> commands;

	// Stop runnning container of same image
	std::string stopContainerCommand = "docker container ls | grep ";
	stopContainerCommand.append(versionLessImage);
	stopContainerCommand.append(" | awk '{print $1}' | xargs docker container stop");
    commands.push_back(stopContainerCommand.c_str());

	// Delete stopped conatainers
    commands.push_back("docker system prune -f");

	// Run new image
	std::string runContinaerCommand = "docker run -d -p ";
	runContinaerCommand.append(port);
	runContinaerCommand.append(":");
	runContinaerCommand.append(port);
	runContinaerCommand.append(" ");
	runContinaerCommand.append(imageName);
    commands.push_back(runContinaerCommand.c_str());

	// Deletes unused images
    commands.push_back("docker image prune -f -a");
	
    SSHConnection* conn = new SSHConnection(commands);
    std::cout << "Created connection succesfully" << std::endl;
    conn->connectAndRun(sshHost, sshPort, sshUser, sshPassword);
    delete conn;
	return 0;
}