#include <vector>
#include <string>
#include "docker.hpp"
#include "ssh_server.hpp"
#include "connection.hpp"

int runDeploymentCommands(char* imageName, char* port, char* sshHost, char* sshPort, char* sshUser, char* sshPassword) {
	std::string imageNameString(imageName);
	const char* versionLessImage = removeVersion(imageNameString).c_str();

	// Pull image
	std::string pullImage = "docker pull " + std::string(imageName);

	// Stop runnning container of same image
	std::string stopContainer = "docker container ls | grep " + std::string(versionLessImage) + " | awk '{print $1}' | xargs docker container stop";

	// Delete stopped conatainers
    std::string deleteContainers = "docker system prune -f";

	// Run new image
	std::string runContainer = "docker run -d -p " + std::string(port) + ":" + std::string(port) + " " + std::string(imageName);

	// Deletes unused images
    std::string deleteImages = "docker image prune -f -a";

	// Links into one command that assures that the process stops if one of the commands fails
	std::string linkedCommand = pullImage + " && " + stopContainer + " && " + deleteContainers + " && " + runContainer + " && " + deleteImages;

	std::vector<const char*> commands;
	commands.push_back(linkedCommand.c_str());

    SSHConnection* conn = new SSHConnection(commands);
    std::cout << "Created connection succesfully" << std::endl;
    conn->connectAndRun(sshHost, sshPort, sshUser, sshPassword);
    delete conn;
	return 0;
}
