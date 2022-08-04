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

COPY  . .

RUN mkdir build \
    && cd build \
    && cmake .. \
    && make \
    && ./ssh_deploy

CMD ["./main", IMAGE_NAME, PORT, SSH_HOST, SSH_PORT, SSH_USER, SSH_PASSWORD]
