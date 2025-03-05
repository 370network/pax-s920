#!/bin/bash
# build.sh - Morc @ 370network
source $PREFIX/../homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build milktrack="
echo "=================="

git clone https://github.com/370network/MilkyTrackerPAX.git repo

if [ ! -f repo/build/src/tracker/libmilkytracker.so ]; then
	echo "MilkyTrackerPAX build process!"
	mkdir repo/build
	cd repo/build
	cmake -DSDL_1=1 -DCMAKE_TOOLCHAIN_FILE=../../build_helper.cmake ..
	cd $BUILD_FOLDER
	make -j$(nproc) -C repo/build/
fi

build_check "build/src/tracker/libmilkytracker.so" "milkytracker"
build_check "build/src/milkyplay/libmilkyplay.so" "libmilkyplay"
build_check "build/src/ppui/libppui.so" "libppui"
