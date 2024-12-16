FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y nasm
RUN command apt-get install -y xorriso
RUN command apt-get install -y grub-pc-bin
RUN command apt-get install -y grub-common

VOLUME /root/env
WORKDIR /root/env