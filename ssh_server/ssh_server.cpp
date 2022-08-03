#include <libssh/libssh.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include "ssh_server.hpp"

int verify_host(ssh_session session)
{
    enum ssh_known_hosts_e state;
    unsigned char *hash = NULL;
    ssh_key srv_pubkey = NULL;
    size_t hlen;
    char buf[10];
    char *hexa;
    char *p;
    int cmp;
    int rc;
    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if (rc < 0) {
        return -1;
    }
    rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0) {
        return -1;
    }
    state = ssh_session_is_known_server(session);
    switch (state) {
        case SSH_KNOWN_HOSTS_OK:
            /* OK */
            break;
        case SSH_KNOWN_HOSTS_CHANGED:
            std::cout << "Host key for server changed: it is now:\n" << std::endl;
            // ssh_print_hexa("Public key hash", hash, hlen);
            std::cout << "For security reasons, connection will be stopped\n" << std::endl;
            ssh_clean_pubkey_hash(&hash);
            return -1;
        case SSH_KNOWN_HOSTS_OTHER:
            std::cout << "The host key for this server was not found but an other type of key exists.\n" << std::endl;
            std::cout << "An attacker might change the default server key to confuse your client into thinking the key does not exist\n" << std::endl;
        	ssh_clean_pubkey_hash(&hash);
            return -1;
        case SSH_KNOWN_HOSTS_NOT_FOUND:
            std::cout << "Could not find known host file.\n" << std::endl;
            std::cout << "If you accept the host key here, the file will be automatically created.\n" << std::endl;
            /* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */
        case SSH_KNOWN_HOSTS_UNKNOWN:
            hexa = ssh_get_hexa(hash, hlen);
            std::cout <<"The server is unknown. Do you trust the host key?\n" << std::endl;
            std::cout << "Public key hash: %s\n" << hexa << std::endl;
            ssh_string_free_char(hexa);
            ssh_clean_pubkey_hash(&hash);
            p = fgets(buf, sizeof(buf), stdin);
            if (p == NULL) {
                return -1;
            }
            cmp = strncasecmp(buf, "yes", 3);
            if (cmp != 0) {
                return -1;
            }
            rc = ssh_session_update_known_hosts(session);
            if (rc < 0) {
                std::cout << "Error %s\n" << strerror(errno) << std::endl;
                return -1;
            }
            break;
        case SSH_KNOWN_HOSTS_ERROR:
            std::cout << "Error %s" << ssh_get_error(session) << std::endl;
            ssh_clean_pubkey_hash(&hash);
            return -1;
    }
    ssh_clean_pubkey_hash(&hash);
    return 0;
}


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
