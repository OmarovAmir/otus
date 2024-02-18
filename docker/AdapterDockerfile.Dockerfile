FROM debian
ARG DEBIAN_FRONTEND=noninteractive
RUN echo 'root:123456' | chpasswd
RUN apt update && \
apt install -y nftables \
               sudo \
               inetutils-ping \
               iproute2 \
               netcat-traditional \
               net-tools \
               tshark \
               tmux \
               gdb \
               htop && \
apt autoclean && \
apt clean
RUN useradd -s /bin/bash -m adapter -p 123456
RUN echo 'adapter:123456' | chpasswd
RUN adduser adapter sudo
USER adapter
WORKDIR /home/adapter