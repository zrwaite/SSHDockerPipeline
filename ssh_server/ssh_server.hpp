#include <libssh/libssh.h>
#include <string>
int connect(char* host, char* port, char* username, char* password);
int show_remote_processes(ssh_session session);