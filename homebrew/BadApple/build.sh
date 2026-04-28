#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build BadApple ="
echo "=================="

git clone https://github.com/paxdevs/paxdemo repo

if [ ! -f repo/cmakelistsupdated ]; then
	cp CMakeLists.txt repo/CMakeLists.txt
	echo "done" > repo/cmakelistsupdated
fi

if [ ! -f repo/mainupdated ]; then
	cp main.cpp repo/main.cpp
	echo "done" > repo/mainupdated
fi

if [ ! -f repo/audioupdated ]; then
	cp audio.cpp repo/audio.cpp
	echo "done" > repo/audioupdated
fi

if [ ! -f repo/libpaxdemo.so ]; then
        echo "paxgbc build processs!"
        cd repo
        cmake -DCMAKE_TOOLCHAIN_FILE=$PAXPATH/toolchain/build/cmake/arm-unknown-linux-gnueabi2.13.cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 .
        make -j$(nproc)
        $STRIP libpaxdemo.so
        cd ..
fi

if [ ! -f BadApple/video.frames ]; then
	mv repo/video.frames BadApple/
fi
if [ ! -f BadApple/audio.opus ]; then
	mv repo/audio.opus BadApple/
fi

build_check "libpaxdemo.so" "BadApple/BadApple"
