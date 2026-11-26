#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="SDL"
PKG_DESC="Development library designed to provide low level access to audio, keyboard, mouse, joystick and graphics"
PKG_VER="1.2.16"
PKG_AUTHOR="Sam Lantinga & Morc <morc@370.network>"

_build() {
	if [ ! -d repo ]; then
		git clone https://github.com/370network/SDL-1.2-pax.git --depth 1 repo
	fi

	if [ ! -f repo/build/.libs/libSDL-1.2.so.0.11.5 ]; then
		cd repo
		./configure --disable-pulseaudio --prefix=$PREFIX --host=$HOST_GLIBC
		make -j$(nproc) install
	fi
}

build_header

build

package_init

_packaging() {
	cp -P $PREFIX/lib/libSDL-1.2.so.0.11.5 $PKG_LIB_DIR
	cp -P $PREFIX/lib/libSDL-1.2.so.0 $PKG_LIB_DIR
}

packaging

package_create
