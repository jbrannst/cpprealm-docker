docker build -t mongo-cpp-base -f example_base.Dockerfile .
docker build -t mongo-cpp-iot -f iot.Dockerfile .
docker run mongo-cpp-iot