FROM nvidia/cuda:11.7.1-cudnn8-devel-ubuntu18.04
ARG DEBIAN_FRONTEND=noninteractive

WORKDIR /app

# Install CMake
RUN apt-get remove -y --purge cmake && \
    apt-get update -y && \
    apt-get install -y gpg wget && \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | \
    gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null && \
    apt-get update && \
    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic main' | \
    tee /etc/apt/sources.list.d/kitware.list >/dev/null && apt-get update && apt-get install -y cmake

# Install further dependencies
RUN apt-get update -y && apt-get autoremove -y && \
    apt-get install -y  libboost-program-options-dev \
                        libopencv-dev \
                        qt5-default \
                        qtmultimedia5-dev

ENTRYPOINT [ "/bin/bash", "-l", "-c" ]