#include <vector>
#include <string>
#include "docker.hpp"
#include "ssh_server.hpp"
#include "connection.hpp"

int runDeploymentCommands(char* dockerHubAccessToken, char* dockerUser, char* imageName, char* imagePort, char* sshHost, char* sshPort, char* sshUser, char* sshPassword) {
	std::string imageNameString(imageName);
	std::string versionLessImage = removeVersion(imageNameString);

	// echo ${{ secrets.DOCKERHUB_ACCESS_TOKEN }} | docker login --username ${{ secrets.DOCKER_USER }} --password-stdin
	std::string dockerHubLogin = "echo " + std::string(dockerHubAccessToken) + " | docker login --username " + std::string(dockerUser) + " --password-stdin";

	// Pull image
	std::string pullImage = "docker pull " + std::string(imageName);

	// Stop runnning container of same image
	std::string stopContainer = "docker container ls | grep " + versionLessImage + " | awk '{print $1}' | xargs docker container stop";

	// Delete stopped conatainers
    std::string deleteContainers = "docker system prune -f";

	// Run new image
	std::string runContainer = "docker run -d -p " + std::string(imagePort) + ":" + std::string(imagePort) + " " + std::string(imageName);

	// Deletes unused images
    std::string deleteImages = "docker image prune -f -a";

	//TODO figure out how to stop if one command fails
	std::vector<const char*> commands;
	commands.push_back(dockerHubLogin.c_str());
	commands.push_back(pullImage.c_str());
	commands.push_back(stopContainer.c_str());
	commands.push_back(deleteContainers.c_str());
	commands.push_back(runContainer.c_str());
	commands.push_back(deleteImages.c_str());

    SSHConnection* conn = new SSHConnection(commands);
    std::cout << "Created connection succesfully" << std::endl;
    conn->connectAndRun(sshHost, sshPort, sshUser, sshPassword);
    delete conn;
	return 0;
}
