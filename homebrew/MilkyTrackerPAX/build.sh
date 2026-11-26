#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="MilkyTrackerPAX"
PKG_DESC="Your favorite excel music player for a payment terminal"
PKG_VER="0.90.87"
PKG_AUTHOR="MilkyTracker contributors, Morc @ 370network"
PKG_DEPENDS="SDL"
PKG_ISAPP=1

build_dependency "SDL"

_build() {
	if [ ! -d repo ]; then
		git clone https://github.com/370network/MilkyTrackerPAX.git --depth 1 repo
	fi

	if [ ! -f repo/cmakefilesupdated ]; then
		cp patches/CMakeLists.txt repo/CMakeLists.txt
		echo "done" > repo/cmakefilesupdated
	fi

	if [ ! -f repo/cmakefilestrackerupdated ]; then
		cp patches/tracker_CMakeLists.txt repo/src/tracker/CMakeLists.txt
		echo "done" > repo/cmakefilestrackerupdated
	fi

	if [ ! -f repo/build/src/tracker/libmilkytracker.so ]; then
		mkdir repo/build
		cd repo/build
		cmake -DSDL_1=1 -DERRATA_3=1 -DCMAKE_CXX_FLAGS="$CXXFLAGS -D__LOWRES__" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_TOOLCHAIN_FILE=$PAXPATH/homebrew/build_helper.cmake ..
		cd $BUILD_FOLDER
		make -j$(nproc) -C repo/build/
	fi
}


build_header

build

package_init

_packaging() {
	cp icon.png $PKG_APP_DIR
	cp "repo/build/src/tracker/libmilkytracker.so" "$PKG_APP_DIR/$_pkg_name.so"
	mkdir "$PKG_APP_DIR/lib/"
	cp "repo/build/src/milkyplay/libmilkyplay.so" "$PKG_APP_DIR/lib/libmilkyplay.so"
	cp "repo/build/src/ppui/libppui.so" "$PKG_APP_DIR/lib/libppui.so"
}

packaging

package_create
