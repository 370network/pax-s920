#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="Suica Reader"
PKG_DESC="Reader of Suica cards. Shows balance as demo."
PKG_VER="1.0"
PKG_AUTHOR="HiImMilan <milan.babuljak@hiimmilan.dev>"
PKG_ISAPP=1

_build() {
	if [ ! -f suica-reader.o ]; then
		make -j$(nproc)
	fi
}

_packaging() {
	cp icon.png $PKG_APP_DIR/
	cp "suica-reader.o" "$PKG_APP_DIR/$_pkg_name"
}

build_header

build

package_init

packaging

package_create