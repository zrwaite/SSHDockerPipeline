#include <libssh/libssh.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>
#include "ssh_server.hpp"
#include "verify.hpp"

int show_remote_processes(ssh_session session)
{
	ssh_channel channel;
	int rc;
	char buffer[256];
	int nbytes;
	channel = ssh_channel_new(session);
	if (channel == NULL)
	{
		std::cout << "Error allocating channel." << std::endl;
		return SSH_ERROR;
	}
	rc = ssh_channel_open_session(channel);
	if (rc != SSH_OK)
	{
		std::cout << "Error opening channel." << std::endl;
		ssh_channel_free(channel);
		return rc;
	}
	rc = ssh_channel_request_exec(channel, "docker ps");
	if (rc != SSH_OK)
	{
		std::cout << "Failed" << std::endl;
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return rc;
	} else {
		std::cout << "Success" << std::endl;
	}
	nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	while (nbytes > 0)
	{
		if (write(1, buffer, nbytes) != (unsigned int) nbytes)
		{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return SSH_ERROR;
		}
		nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
	}
	if (nbytes < 0)
	{
		ssh_channel_close(channel);
		ssh_channel_free(channel);
		return SSH_ERROR;
	}
	ssh_channel_send_eof(channel);
	ssh_channel_close(channel);
	ssh_channel_free(channel);
	return SSH_OK;
}

int connect(char* host, char* port, char* username, char* password)
{
  // Create a new ssh session
  ssh_session my_ssh_session;
  my_ssh_session = ssh_new();
  if (my_ssh_session == NULL)
    exit(-1);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, username);
  int rc = ssh_connect(my_ssh_session);
  if (rc != SSH_OK)
  {
    fprintf(stderr, "Error connecting to host: %s\n",
            ssh_get_error(my_ssh_session));
    exit(-1);
  }

    if (verify_host(my_ssh_session) < 0)
    {
        std::cout << "Failed to verify host" << std::endl;
        ssh_disconnect(my_ssh_session);
        ssh_free(my_ssh_session);
        exit(-1);
    }

  rc = ssh_userauth_password(my_ssh_session, NULL, password);
  if (rc != SSH_AUTH_SUCCESS)
  {
    fprintf(stderr, "Error authenticating with password: %s\n",
            ssh_get_error(my_ssh_session));
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    exit(-1);
  }
  show_remote_processes(my_ssh_session);

    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
  return 0;
}
