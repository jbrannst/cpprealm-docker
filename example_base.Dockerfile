FROM ubuntu:22.04 AS base-x86

# Install dependencies
RUN apt update &&  apt install -y --no-install-recommends build-essential \
    git libcurl4-openssl-dev libuv1-dev wget libfuse-dev \
    libprocps-dev libssl-dev zlib1g-dev cmake ca-certificates fuse \
    ninja-build \
    sudo \
    libcpprest-dev

# Install RealmSDK
WORKDIR /opt
RUN git clone https://github.com/realm/realm-cpp.git && \
    cd realm-cpp && \
    git submodule update --init --recursive

RUN cd realm-cpp && mkdir -p /build.release
WORKDIR /opt/realm-cpp/build.release
RUN cmake -DCMAKE_BUILD_TYPE=Release ..
RUN sudo cmake --build . --target install
WORKDIR ../../