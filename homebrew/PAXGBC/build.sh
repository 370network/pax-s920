#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build paxgbc  ="
echo "=================="

git clone https://github.com/rhgndf/paxgbc repo

if [ ! -f repo/cmakelistsupdated ]; then
	cp CMakeLists.txt repo/CMakeLists.txt
	echo "done" > repo/cmakelistsupdated
fi

if [ ! -f repo/mainupdated ]; then
	cp main.cpp repo/main.cpp
	echo "done" > repo/mainupdated
fi

if [ ! -f repo/libpaxgbc.so ]; then
        echo "paxgbc build process!"
        cd repo
        cmake -DCMAKE_TOOLCHAIN_FILE=$PAXPATH/toolchain/build/cmake/arm-unknown-linux-gnueabi2.13.cmake .
        make -j$(nproc)
        $STRIP libpaxgbc.so
        cd ..
fi


build_check "libpaxgbc.so" "PAXGBC/PAXGBC"