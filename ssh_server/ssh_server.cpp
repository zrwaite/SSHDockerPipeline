#include <libssh/libssh.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include "ssh_server.hpp"

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
