#include <libssh/libssh.h>
#include <iostream>
#include <vector>
#include "connection.hpp"
#include "verify.hpp"

 SSHConnection::SSHConnection(std::vector<const char*> commands) {
    this->commands = commands;
}

int SSHConnection::connectAndRun(char *host, char *port, char *username, char *password) {
    this->session = ssh_new();
    // Tries to create ssh session
    if (this->session == NULL) {
        std::cout << "Session was initialized to NULL" << std::endl;
        exit(-1);
    }

    //Adds options to the ssh session
    ssh_options_set(this->session, SSH_OPTIONS_HOST, host);
    ssh_options_set(this->session, SSH_OPTIONS_USER, username);

    //Tries to connect to the host
    if (ssh_connect(this->session) != SSH_OK)
    {
        fprintf(stderr, "Error connecting to host: %s\n", ssh_get_error(this->session));
        exit(-1);
    }

    //Verifies valididy of host
    if (verify_host(this->session) < 0)
    {
        std::cout << "Failed to verify host" << std::endl;
        ssh_disconnect(this->session);
        ssh_free(this->session);
        exit(-1);
    }

    //Enter password for authenticated connection
    if (ssh_userauth_password(this->session, NULL, password) != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(this->session));
        ssh_disconnect(this->session);
        ssh_free(this->session);
        exit(-1);
    }
    return this->runCommands();
}
SSHConnection::~SSHConnection() {
    ssh_disconnect(this->session);
    ssh_free(this->session);
    std::cout << "Deleting connection" << std::endl;
}

void SSHConnection::createChannel() {
    this->channel = ssh_channel_new(this->session);
    if (this->channel == NULL)
    {
        std::cout << "Error allocating channel." << std::endl;
        exit(-1);
    }
    if (ssh_channel_open_session(this->channel) != SSH_OK)
    {
        std::cout << "Error opening channel." << std::endl;
        ssh_channel_free(this->channel);
        exit(-1);
    }
}

void SSHConnection::deleteChannel() {
    ssh_channel_close(this->channel);
    ssh_channel_free(this->channel);
    std::cout << "Channel deleted" << std::endl;
}

int SSHConnection::runCommands() {
    for (const char* command : this->commands) {
        this->createChannel();
        if (this->runCommand(command) != SSH_OK) {
            this->deleteChannel();
            return SSH_ERROR;
        }
        this->deleteChannel();
    }
    return SSH_OK;
}

int SSHConnection::runCommand(const char* command){
    std::cout << "running command: " << command << std::endl;
    char buffer[256];
    int nbytes;
    int rc = ssh_channel_request_exec(this->channel, command);
    if (rc != SSH_OK)
    {
        std::cout << "Command " << command << " failed" << std::endl;
        return rc;
    }
    std::cout << "Command success" << std::endl;
    nbytes = ssh_channel_read(this->channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0)
    {
        if (write(1, buffer, nbytes) != (unsigned int) nbytes)
        {
            return SSH_ERROR;
        }
        nbytes = ssh_channel_read(this->channel, buffer, sizeof(buffer), 0);
    }
    if (nbytes < 0) return SSH_ERROR;
    ssh_channel_send_eof(this->channel);
    return SSH_OK;
}