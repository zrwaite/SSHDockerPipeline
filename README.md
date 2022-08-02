# Multi-Docker DigitalOcean Droplet Pipeline

1. Pass image name and port to deployImage.sh
1. It finds running container with that name
1. If found, it stops it
1. Cleans up loose containers
1. Runs new image at port
1. Cleans up loose images

g++ getContainerId.cpp -o getContainerId
sudo chmod +x test.sh
