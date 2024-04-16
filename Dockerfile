FROM ubuntu:jammy

ENV DEBIAN_FRONTEND noninteractive
ARG BRANCH_NAME

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

# Download the Android NDK.
RUN \
  wget https://dl.google.com/android/repository/android-ndk-r25c-linux.zip && \
  unzip android-ndk-r25c-linux.zip && \
  rm android-ndk-r25c-linux.zip
ENV ANDROID_NDK_HOME /android-ndk-r25c

# This is needed by OpenSSL because for some reason it uses PATH to find the Android NDK compilers (and obviously
# this will only work for linux-x86_64 images).
ENV PATH $ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH

# Clone the repository.
RUN git clone https://github.com/hashgraph/hedera-protobufs-cpp.git

# Change the working directory to that of the cloned repository.
WORKDIR /hedera-sdk-cpp

# Checkout the working branch.
RUN git checkout origin/$BRANCH_NAME

# Update the submodule.
RUN git submodule update --init

# Build
RUN cmake --preset android-arm64-release
RUN cmake --preset android-arm64-debug
RUN cmake --build --preset android-arm64-release
RUN cmake --build --preset android-arm64-debug

# TODO:
# The steps below are steps needed to test the Android library executables. Currently, the Android emulator is unable to
# run in a linux-x86_64 environment because it is built for arm64. If there is interest in testing the Android library
# in CI, this issue of where to run the Android emulator will need to be figured out (must be in an arm64 environment,
# arm64 docker container has been tested but doesn't work). The steps here should be the general workflow of how to get
# the emulator up and running and the test executables ran.

# Download Android SDK
#WORKDIR /
#RUN \
#  mkdir android-sdk && \
#  cd android-sdk && \
#  wget https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip && \
#  unzip commandlinetools-linux-11076708_latest.zip && \
#  rm commandlinetools-linux-11076708_latest.zip && \
#  # For some reason the SDK download doesn't put the cmdline-tools in a 'latest' folder even though its required by the
#  # sdkmanager, so some directory rearranging is required here.
#  mkdir latest && \
#  mv cmdline-tools/* latest && \
#  mv latest cmdline-tools

# Add environment variables to help run tools.
#ENV ANDROID_HOME /android-sdk
#ENV PATH $ANDROID_HOME/cmdline-tools/latest/bin:$ANDROID_HOME/emulator:$ANDROID_HOME/platform-tools:$ANDROID_HOME
#/build-tools/29.0.2:$PATH

# Download an Android platform on which to test.
#RUN yes | sdkmanager --licenses
#RUN sdkmanager --update
#RUN sdkmanager \
#    "emulator" \
#    "platforms;android-21" \
#    "platform-tools" \
#    "system-images;android-21;default;arm64-v8a" \
#    "build-tools;29.0.2"
#RUN echo | avdmanager create avd --name test-avd --package "system-images;android-21;default;arm64-v8a"
#RUN emulator -avd test-avd -no-audio -no-window

# TODO:
# The steps below are remnants of the effort that was put into making the examples Android APKs. This has since been
# abandoned, but if this is looked at again then here are some remnants of that work to pick up.

# RUN apt-get -y --no-install-recommends install openjdk-17-jdk
# ENV JAVA_HOME /usr/lib/jvm/java-17-openjdk-amd64

#WORKDIR /hedera-sdk-cpp/src/sdk/examples/CreateAccountExample
#RUN mkdir -p build/gen build/obj build/apk/lib/arm64-v8a build/apk/assets
#RUN aapt package -f -m -J build/gen -S res -M AndroidManifest.xml -I $ANDROID_HOME/platforms/android-21/android.jar
#RUN javac -bootclasspath $JAVA_HOME/jre/lib/rt.jar -classpath $ANDROID_HOME/platforms/android-21/android.jar -d \
#    build/obj build/gen/com/hedera/hashgraph/sdk/cpp/examples/R.java \
#    src/main/java/com/hedera/hashgraph/sdk/cpp/examples/CreateAccountExample.java -source 1.7 -target 1.7
#RUN dx --dex --output=build/apk/classes.dex build/obj
#RUN cp \
#    /hedera-sdk-cpp/build/android-arm64-release/src/sdk/examples/Release/libhedera-sdk-cpp-create-account-example.so \
#    build/apk/lib/arm64-v8a/
#RUN cp -r /hedera-sdk-cpp/addressbook build/apk/assets
#RUN cp -r /hedera-sdk-cpp/config build/apk/assets
#RUN aapt package -f -M AndroidManifest.xml -S res -I $ANDROID_HOME/platforms/android-21/android.jar -F \
#    build/hedera-sdk-cpp-create-account-example-apk.unsigned.apk build/apk
#RUN zipalign -f -p 4 build/hedera-sdk-cpp-create-account-example-apk.unsigned.apk \
#    build/hedera-sdk-cpp-create-account-example-apk.aligned.apk
#RUN { echo testkey; echo testkey; echo; echo; echo; echo; echo; echo; echo y; } | keytool -genkeypair -keystore \
#    keystore.jks -alias androidkey -validity 10000 -keyalg RSA -keysize 2048
#RUN echo testkey | apksigner sign --ks keystore.jks --ks-key-alias androidkey --out \
#    build/hedera-sdk-cpp-create-account-example-apk.apk build/hedera-sdk-cpp-create-account-example-apk.aligned.apk

