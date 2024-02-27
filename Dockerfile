FROM ubuntu:jammy

ENV DEBIAN_FRONTEND noninteractive

RUN \
  apt-get update && \
  apt-get -y upgrade

RUN \
  apt-get -y --no-install-recommends install \
    build-essential \
    ca-certificates \
    cmake \
    curl \
    git \
    libc6-dbg \
    libgtest-dev \
    linux-libc-dev \
    ninja-build \
    pkg-config \
    python3 \
    tar \
    unzip \
    vim \
    wget \
    zip

# Download Android NDK
RUN \
  wget https://dl.google.com/android/repository/android-ndk-r26c-linux.zip && \
  unzip android-ndk-r26c-linux.zip && \
  rm -rf android-ndk-r26c-linux.zip

ENV ANDROID_NDK_HOME /android-ndk-r26c
ENV VCPKG_FORCE_SYSTEM_BINARIES 1

# This is needed by OpenSSL because for some reason it uses PATH to find the Android NDK compilers (and obviously
# this will only work for linux-x86_64 images).
ENV ANDROID_NDK_ROOT $ANDROID_NDK_HOME
ENV PATH $ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

# Clone your repository
RUN git clone https://github.com/hashgraph/hedera-sdk-cpp.git

# Change working directory to cloned repo
WORKDIR /hedera-sdk-cpp

# Change to working branch
RUN git checkout 00660-add-android-and-ios-builds

# Update submodule
RUN git submodule update --init

# Build
#RUN cmake --preset android-arm64-release
#RUN cmake --build --preset android-arm64-release

#RUN git clone https://github.com/microsoft/vcpkg
#WORKDIR vcpkg
#RUN ./bootstrap-vcpkg.sh

# Use Ubuntu Jammy as the base image
#FROM ubuntu:jammy

# Avoid prompts from apt
#ENV DEBIAN_FRONTEND=noninteractive

# Update and install necessary packages
#RUN apt-get update && apt-get install -y \
#git \
#cmake \
#ninja-build \
#pkg-config \
#libc6-dbg \
#libgtest-dev \
#linux-libc-dev \
#build-essential \
#curl \
#zip \
#unzip \
#tar

# Clone your repository
#WORKDIR /home
#RUN git clone https://github.com/hashgraph/hedera-sdk-cpp.git

# Change working directory to cloned repo
#WORKDIR /home/hedera-sdk-cpp

# Update submodule
#RUN git submodule update --init

# Ensure vcpkg binary cache exists
#RUN mkdir -p /vcpkg_cache
#ENV VCPKG_FORCE_SYSTEM_BINARIES=vcpkg_cache/

# Bootstrap vcpkg
#WORKDIR /home/hedera-sdk-cpp/vcpkg
#RUN ./bootstrap-vcpkg.sh

# Build
#WORKDIR /home/hedera-sdk-cpp
#RUN cmake --preset linux-x64-release
#RUN cmake --build --preset linux-x64-release
