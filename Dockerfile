# Use the latest Ubuntu LTS release as the base image
FROM ubuntu:20.04

# Update the package index and install some basic packages
RUN apt-get update && \
    apt-get install -y \
    curl \
    wget \
    git \
    vim \
    nano \
    iproute2 \
    build-essential \
    linux-headers-generic \
    kmod \
    && rm -rf /var/lib/apt/lists/*

# Copy the script to the container

RUN mkdir /app
COPY test.c /app/test.c
COPY Makefile /app/Makefile
WORKDIR /app

# Set the default command to launch a bash shell
CMD ["/bin/bash"]
