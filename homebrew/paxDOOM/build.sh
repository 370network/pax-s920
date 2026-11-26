#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="paxDOOM"
PKG_DESC="A DOOM port based on fbdoom"
PKG_VER="1.0.0"
PKG_AUTHOR="ID Software, maximevince, Morc @ 370network"
PKG_ISAPP=1

_build() {
	if [ ! -d repo ]; then
		git clone https://github.com/370network/paxDOOM.git --depth 1 repo
	fi

	export CROSS_COMPILE=$TOOLCHAIN/bin/$HOST-

	if [ ! -f repo/fbdoom/fbdoom ]; then
		make -j$(nproc) -C repo/fbdoom/
	fi
}

_packaging() {
	cp icon.png $PKG_APP_DIR
	cp "repo/fbdoom/fbdoom" "$PKG_APP_DIR/$_pkg_name.so"
}

build_header

build

package_init

packaging

package_create
