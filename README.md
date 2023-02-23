# cpprealm-docker

Docker-based build for realm C++ SDK. The example application is writing a new object to the realm every second, which gets automatically synced to Atlas. 

Installation instructions below:


```
docker build -t mongo-cpp-base -f example_base.Dockerfile .
docker build -t mongo-cpp-iot -f iot.Dockerfile .
docker run mongo-cpp-iot
```