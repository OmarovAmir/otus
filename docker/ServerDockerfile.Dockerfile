FROM debian
ARG DEBIAN_FRONTEND=noninteractive
RUN echo 'root:123456' | chpasswd
RUN apt update && apt install -y sudo inetutils-ping iproute2 netcat-traditional net-tools && apt autoclean && apt clean
RUN useradd -s /bin/bash -m server -p 123456
RUN echo 'server:123456' | chpasswd
RUN adduser server sudo
USER server
WORKDIR /home/server