#include <libssh/libssh.h>
#include <vector>

class SSHConnection {
	public:
        SSHConnection(std::vector<char *> commands);
        ~SSHConnection();
        int connectAndRun(char* host, char* port, char* username, char* password);
	private:
        void createChannel();
        void deleteChannel();
        int runCommands();
        int runCommand(char* command);
        ssh_channel channel;
        std::vector<char*> commands;
        ssh_session session;
};