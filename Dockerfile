FROM ubuntu:latest

ARG image_name
ARG port
ARG ssh_host
ARG ssh_port
ARG ssh_user
ARG ssh_password

ENV IMAGE_NAME=$image_name
ENV PORT=$port
ENV SSH_HOST=$ssh_host
ENV SSH_PORT=$ssh_port
ENV SSH_USER=$ssh_user
ENV SSH_PASSWORD=$ssh_password

WORKDIR /app

RUN set -ex; \
    apt-get update; \
    apt-get install -y cmake; \
    apt-get install -y clang; \
    apt-get install -y libssh-dev; 

COPY  . .

RUN mkdir build \
    && cd build \
    && cmake .. \
    && make 

# CMD ["/bin/bash"]

CMD ["./build/ssh_deploy", "${image_name}", "${port}", "${ssh_host}", "${ssh_port}", "${ssh_user}", "${ssh_password}"]
