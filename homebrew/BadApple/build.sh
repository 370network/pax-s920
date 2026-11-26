#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="Bad Apple"
PKG_DESC="Bad Apple Demo App"
PKG_VER="1.0.0"
PKG_AUTHOR="rhgndf"
PKG_ISAPP=1

_build() {
	if [ ! -d repo ]; then
		git clone https://github.com/paxdevs/paxdemo --depth 1 repo
	fi

	if [ ! -f repo/cmakelistsupdated ]; then
		cp patches/CMakeLists.txt repo/CMakeLists.txt
		echo "done" > repo/cmakelistsupdated
	fi

	if [ ! -f repo/mainupdated ]; then
		cp patches/main.cpp repo/main.cpp
		echo "done" > repo/mainupdated
	fi

	if [ ! -f repo/audioupdated ]; then
		cp patches/audio.cpp repo/audio.cpp
		echo "done" > repo/audioupdated
	fi

	if [ ! -f repo/libpaxdemo.so ]; then
	        echo "BadApple build processs!"
	        cd repo
	        cmake -DCMAKE_TOOLCHAIN_FILE=../../build_helper.cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 .
	        make -j$(nproc)
	        $STRIP libpaxdemo.so
	        cd ..
	fi
}

build_header

build

package_init

_packaging() {
	cp icon.png $PKG_APP_DIR
	cp repo/video.frames $PKG_APP_DIR
	cp repo/audio.opus $PKG_APP_DIR
	cp "repo/libpaxdemo.so" "$PKG_APP_DIR/$_pkg_name.so"
}

packaging

package_create