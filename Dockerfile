# Build Stage
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y cmake clang
#RUN apt update && apt install -y make
RUN DEBIAN_FRONTEND=noninteractive apt-get -qq install zlib1g-dev
## Add source code to the build stage.
COPY . /libxlsxwriter/
#RUN ls -a
RUN cd /libxlsxwriter/examples && make

#RUN make
## TODO: ADD YOUR BUILD INSTRUCTIONS HERE.
#RUN make

# Package Stage
FROM --platform=linux/amd64 ubuntu:21.10

## TODO: Change <Path in Builder Stage>
COPY --from=builder /libxlsxwriter/examples/fuzzme /
