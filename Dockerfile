FROM ubuntu:22.04 as build
RUN apt update && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt install -y tzdata \
    build-essential cmake pkg-config libboost-all-dev libcurl4-openssl-dev libgtest-dev libssl-dev git
RUN mkdir -p /opt/datascrapper
WORKDIR /opt/datascrapper
COPY src src
RUN mkdir scripts
COPY scripts/build.sh scripts/test.sh scripts/
COPY test test
COPY CMakeLists.txt CMakeLists.txt
RUN /bin/bash scripts/build.sh
FROM ubuntu:22.04
RUN apt update && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt install -y tzdata \
    libboost-all-dev libcurl4-openssl-dev libssl-dev
RUN mkdir -p /opt/datascrapper
WORKDIR /opt/datascrapper
COPY --from=build /opt/datascrapper/DataScrapper DataScrapper
CMD ./DataScrapper