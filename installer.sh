#!/usr/bin/env bash

# Build the program from source
mkdir build
cd build
cmake ../
make
rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile

# Copy the executable
cd build/
cp Xerxes /opt/Xerxes
cp useragents /opt/Xerxes
ln -sf /opt/Xerxes/Xerxes /usr/bin

echo "Xerxes is installed"