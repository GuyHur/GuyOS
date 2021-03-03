FROM ubuntu

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y g++ binutils libc6-dev-i386
RUN apt-get install -y grub-common xorriso make

VOLUME /root/env
WORKDIR /root/env