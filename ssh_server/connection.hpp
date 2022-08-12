#include <libssh/libssh.h>
#include <vector>

class SSHConnection {
	public:
        SSHConnection(std::vector<const char*> commands);
        ~SSHConnection();
        int connectAndRun(char* host, char* port, char* username, char* password);
	private:
        void createChannel();
        void deleteChannel();
        int runCommands();
        int runCommand(const char* command);
        ssh_channel channel;
        std::vector<const char*> commands;
        ssh_session session;
};