#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="PAXBoard"
PKG_DESC="Homebrew Launcher"
PKG_VER="1.2"
PKG_AUTHOR="370network"

_build() {
	if [ ! -f Launcher.so ]; then
		make
	fi
}

_packaging() {
	mkdir -p $PKG_BASE_DIR/paxboard/lib
	cp Launcher.so $PKG_BASE_DIR/paxboard/lib/libosal.so
}

build_header

build

package_init

packaging

package_create
