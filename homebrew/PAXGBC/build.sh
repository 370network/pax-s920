#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="PAXGBC"
PKG_DESC="Game Boy Color Emulator"
PKG_VER="1.0.0"
PKG_AUTHOR="rhgndf"
PKG_ISAPP=1

_build() {
	if [ ! -d repo ]; then
			git clone https://github.com/rhgndf/paxgbc --depth 1 repo
	fi

	if [ ! -f repo/cmakelistsupdated ]; then
		cp CMakeLists.txt repo/CMakeLists.txt
		echo "done" > repo/cmakelistsupdated
	fi

	if [ ! -f repo/mainupdated ]; then
		cp main.cpp repo/main.cpp
		echo "done" > repo/mainupdated
	fi

	if [ ! -f repo/libpaxgbc.so ]; then
		cd repo
		cmake -DCMAKE_TOOLCHAIN_FILE=$PAXPATH/homebrew/build_helper.cmake .
		make -j$(nproc)
	fi
}

_packaging() {
	cp icon.png $PKG_APP_DIR
	cp bbmode1_8x8.png $PKG_APP_DIR
	cp "repo/libpaxgbc.so" "$PKG_APP_DIR/$_pkg_name.so"
	#cp -r roms $PKG_DEVICE_DIR/ - we aren't providing roms anymore in the toolchain repo, atleast for now :)
}

build_header

build

package_init

packaging

package_create
