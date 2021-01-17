#!/bin/bash

rm -rfv CMakeFiles
rm -v CMakeCache.txt
rm -v Makefile
rm -v cmake_install.cmake
rm -v libVirtualPanic.a

echo -e "\033[33msudo rm -rfv /usr/local/include/VirtualPanic\033[0m"
echo -e "\033[33msudo rm -v /usr/local/lib/libVirtualPanic.a\033[0m"
sudo rm -rfv /usr/local/include/VirtualPanic
sudo rm -v /usr/local/lib/libVirtualPanic.a

./build.sh install

