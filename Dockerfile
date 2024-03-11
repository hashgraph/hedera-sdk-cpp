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
    openjdk-17-jdk \
    pkg-config \
    python3 \
    tar \
    unzip \
    vim \
    wget \
    zip

# Download Android NDK
RUN \
  wget https://dl.google.com/android/repository/android-ndk-r25c-linux.zip && \
  unzip android-ndk-r25c-linux.zip && \
  rm android-ndk-r25c-linux.zip
ENV ANDROID_NDK_HOME /android-ndk-r25c

# This is needed by OpenSSL because for some reason it uses PATH to find the Android NDK compilers (and obviously
# this will only work for linux-x86_64 images).
ENV PATH $ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

# Download Android SDK
#RUN \
#  mkdir android-sdk && \
#  cd android-sdk && \
#  wget https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip && \
#  unzip commandlinetools-linux-11076708_latest.zip && \
#  rm commandlinetools-linux-11076708_latest.zip && \
  # For some reason the SDK download doesn't put the cmdline-tools in a 'latest' folder even though its required by the
  # sdkmanager, so some directory rearranging is required here.
#  mkdir latest && \
#  mv cmdline-tools/* latest && \
#  mv latest cmdline-tools

# Add environment variables to help run tools.
#ENV ANDROID_HOME /android-sdk
#ENV PATH $ANDROID_HOME/cmdline-tools/latest/bin:$ANDROID_HOME/emulator:$ANDROID_HOME/platform-tools:$PATH

# Download an Android platform on which to test.
#RUN yes | sdkmanager --licenses
#RUN sdkmanager --update
#RUN sdkmanager "emulator" "platforms;android-21" "platform-tools" "system-images;android-21;default;arm64-v8a"
#RUN echo | avdmanager create avd --name test-avd --package "system-images;android-21;default;arm64-v8a"
#RUN emulator -avd test-avd -no-audio -no-window

RUN git clone https://github.com/hashgraph/hedera-protobufs-cpp.git
WORKDIR /hedera-protobufs-cpp
RUN git checkout 00042-add-android-and-ios-builds
ENV VCPKG_FORCE_SYSTEM_BINARIES 1
RUN git submodule update --init
RUN cmake --preset android-arm64-release
RUN cmake --build --preset android-arm64-release
WORKDIR /

# Clone your repository
RUN git clone https://github.com/hashgraph/hedera-sdk-cpp.git

# Change working directory to cloned repo
WORKDIR /hedera-sdk-cpp

# Change to working branch
RUN git checkout 00660-add-android-and-ios-builds

# Update vcpkg submodule
RUN git submodule update --init

# Build
RUN cmake --preset android-arm64-release
RUN cmake --build --preset android-arm64-release
