#!/bin/bash
# build.sh - Morc @ 370network

echo "=================="
echo "370network paxdevs"
echo "= build paxDOOM  ="
echo "=================="

git clone https://github.com/370network/paxDOOM.git repo

export CROSS_COMPILE=$PREFIX/bin/arm-none-linux-gnueabi-

if [ ! -f repo/fbdoom/fbdoom ]; then
	echo "paxDOOM build process!"
	make -j$(nproc) -C repo/fbdoom/
fi

if [ -f repo/fbdoom/fbdoom ]; then
        echo "paxDOOM built successfully!"
        mv repo/fbdoom/fbdoom paxdoom.so
	echo "file: paxdoom.so"
fi
