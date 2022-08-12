FROM ubuntu:latest


ARG docker_access_token
ARG docker_user
ARG image_name
ARG port
ARG ssh_host
ARG ssh_port
ARG ssh_user
ARG ssh_password

ENV DOCKER_ACCESS_TOKEN=$docker_access_token
ENV DOCKER_USER=$docker_user
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
    && make \
    && chmod +x ./ssh_deploy



CMD ["sh", "-c", "./build/ssh_deploy ${DOCKER_ACCESS_TOKEN} ${DOCKER_USER} ${IMAGE_NAME} ${PORT} ${SSH_HOST} ${SSH_PORT} ${SSH_USER} ${SSH_PASSWORD} "]