FROM mongo-cpp-base
EXPOSE 9000

RUN apt install -y curl

ADD cpprealm-example cpprealm-example
WORKDIR cpprealm-example
RUN mkdir build.release
WORKDIR build.release
RUN cmake -GNinja -DCMAKE_BUILD_TYPE=release ..
RUN ninja
ENTRYPOINT ./cpprealm_iot

