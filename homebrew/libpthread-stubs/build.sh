#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libpthread-stubs"
PKG_DESC="The libpthread-stubs package provides weak aliases for pthread functions not provided in libc or otherwise available by default"
PKG_VER="0.5"
PKG_AUTHOR="Jamey Sharp, Josh Triplett et al"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xcb.freedesktop.org/dist/$PKG_NAME-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f $PREFIX/lib/pkgconfig/pthread-stubs.pc ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

build_header

build

wont_package