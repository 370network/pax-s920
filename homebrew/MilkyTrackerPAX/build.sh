#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build milktrack="
echo "=================="

git clone https://github.com/370network/MilkyTrackerPAX.git repo

if [ ! -f repo/cmakefilesupdated ]; then
	cp CMakeLists.txt repo/CMakeLists.txt
	echo "done" > repo/cmakefilesupdated
fi

if [ ! -f repo/cmakefilestrackerupdated ]; then
	cp tracker_CMakeLists.txt repo/src/tracker/CMakeLists.txt
	echo "done" > repo/cmakefilestrackerupdated
fi

if [ ! -f repo/build/src/tracker/libmilkytracker.so ]; then
	echo "MilkyTrackerPAX build process!"
	mkdir repo/build
	cd repo/build
	#export OBGGCC_STATIC_RUNTIME=true
	#export OBGGCC_BUILTIN_LOADER=true
	cmake -DSDL_1=1 -DERRATA_3=1 -DCMAKE_CXX_FLAGS="-D__LOWRES__" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_TOOLCHAIN_FILE=../../build_helper.cmake ..
	cd $BUILD_FOLDER
	make -j$(nproc) -C repo/build/
	$STRIP repo/build/src/tracker/libmilkytracker.so
	$STRIP repo/build/src/milkyplay/libmilkyplay.so
	$STRIP repo/build/src/ppui/libppui.so
fi

build_check "build/src/tracker/libmilkytracker.so" "MilkyTrackerPAX/MilkyTrackerPAX"
build_check "build/src/milkyplay/libmilkyplay.so" "MilkyTrackerPAX/lib/libmilkyplay"
build_check "build/src/ppui/libppui.so" "MilkyTrackerPAX/lib/libppui"
