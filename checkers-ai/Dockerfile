#
# Ubuntu Dockerfile
#
# https://github.com/dockerfile/ubuntu
#

FROM ubuntu:14.04

RUN \
  sed -i 's/# \(.*multiverse$\)/\1/g' /etc/apt/sources.list && \
  apt-get update && \
  apt-get -y upgrade && \
  apt-get install -y build-essential && \
  apt-get install -y software-properties-common && \
  apt-get install -y byobu curl git htop man unzip vim wget && \
  rm -rf /var/lib/apt/lists/*

RUN apt-get update -y && apt-get upgrade -y

# Set environment variables.
ENV HOME /root

# Define working directory.
WORKDIR /root

RUN apt-get install -y cmake
RUN git clone https://www.github.com/williamh890/checkers-ai
RUN cmake --version

RUN echo 'set(LIBSTDCPPFS_PATH "/usr/lib/gcc/x86_64-linux-gnu/5/libstdc++fs.a")' > checkers-ai/checkers-ai/libstdcppfsPath.txt

RUN cd checkers-ai/checkers-ai && ./install.sh && cmake .
