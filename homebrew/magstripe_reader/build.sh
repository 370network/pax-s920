#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="Magstripe Reader"
PKG_DESC="A MSR test app"
PKG_VER="1.0.0"
PKG_AUTHOR="HiImMilan <milan.babuljak@hiimmilan.dev>"
PKG_ISAPP=1

_build() {
	if [ ! -f magstripe_reader.o ]; then
		make -j$(nproc)
	fi
}

_packaging() {
	cp icon.png $PKG_APP_DIR
	cp "magstripe_reader.o" "$PKG_APP_DIR/$_pkg_name"
}

build_header

build

package_init

packaging

package_create
