FROM ubuntu:16.04

# Install packages
RUN \
    sed -i 's/# \(.*multiverse$\)/\1/g' /etc/apt/sources.list && \
    apt-get update                                            && \
    apt-get -y upgrade                                        && \
    apt-get install -y build-essential                        && \
    apt-get install -y software-properties-common             && \
    apt-get install -y byobu curl git htop man unzip vim wget && \
    rm -rf /var/lib/apt/lists/*                               && \
    apt-get update -y && apt-get upgrade -y                   && \
    apt-get install -y cmake

ENV HOME /root

RUN git clone https://www.github.com/williamh890/checkers-ai
WORKDIR /checkers-ai/checkers-ai

# Build checker-ai
RUN \
    ./install.sh                                                 && \
    echo 'set(LIBSTDCPPFS_PATH "/usr/lib/gcc/x86_64-linux-gnu/5/libstdc++fs.a")' > libstdcppfsPath.txt && \
    ./fast-build.sh

# Setup networks
RUN \
    git clone -b tournament https://www.github.com/williamh890/networks     && \
    apt-get install -y uuid                                                 && \
    echo $(uuid) > user.me                                                  && \
    cd networks && git checkout -b $(cat ../user.me) && cd ..               && \
    ./manager.out 0

# Setup tournament
WORKDIR /checkers-ai/checkers-ai/tournament
COPY docker-tounament.cfg tournament.cfg

RUN \
    apt-get install python3-pip -y && \
    pip3 install -r requirements.txt

RUN \
    echo "[user]\n    email = wbhorn@alaska.edu\n    name = William Horn" > ~/.gitconfig

CMD ["python3", "director.py"]


