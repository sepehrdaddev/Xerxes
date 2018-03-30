#!/usr/bin/env bash

# Download the dependencies
apt install build-essentials gcc g++ cmake clang llvm libssl-dev openssl-dev

# Build the program from source
mkdir build
cd build
cmake ../
make
rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile

# Copy the executable
mkdir /opt/Xerxes
cp Xerxes /opt/Xerxes
cp useragents /opt/Xerxes
ln -sf /opt/Xerxes/Xerxes /usr/bin

echo "Xerxes is installed"