# Multi-Container SSH Server Pipeline

1. Pass `image_name`, `port`, `ssh_host`, `ssh_port`, `ssh_user`, `ssh_password`
1. SSH to `ssh_host`:`ssh_port` as `ssh_user` with `ssh_password`
1. Pulls the new image from the registry.
1. It finds running container with that image_name, of any version
1. If found, it stops that container.
1. Cleans up loose containers
1. Runs new image at port specified
1. Cleans up loose images
1. Disconnects SSH
