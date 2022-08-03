#include <libssh/libssh.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "docker.hpp"
#include "ssh_server.hpp"
#include "verify.hpp"

int main()
{
  // Create a new ssh session
  ssh_session my_ssh_session;
  my_ssh_session = ssh_new();
  if (my_ssh_session == NULL)
    exit(-1);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, "www.zacwaite.xyz");
  ssh_options_set(my_ssh_session, SSH_OPTIONS_USER, "zac");
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

  char *password = "";
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
  std::cout << removeVersion("hello:latest") << std::endl;
  return 0;
}
