#!/bin/sh

INSTALL_PREFIX="$PWD/install"

# build hyodermic first
if [ ! -e "hypodermic" ]; then
	git clone https://github.com/karolherbst/hypodermic.git hypodermic
	mkdir hypodermic/build
else
	cd hypodermic
	git pull origin master
	cd ..
fi

cd hypodermic/build
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX
make install
cd ../..

# check for files
if [ ! -e "files" ]; then
	git clone https://github.com/karolherbst/Gamekeeper-Files.git files
	mkdir files/build
else
	cd files
	git pull origin master
	cd ..
fi

cd files/build
cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX
make install
cd ../..

# build GameKeeper

if [ ! -e "build" ]; then
	mkdir build
	cd build
	cmake .. -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX
else
	cd build
	cmake ..
fi

make install
