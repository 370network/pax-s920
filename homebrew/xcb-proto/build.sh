#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="xcb-proto"
PKG_DESC="This package provides the XML-XCB protocol descriptions that libxcb uses to generate the majority of its code and API"
PKG_VER="1.17.0"
PKG_AUTHOR="Jamey Sharp, Josh Triplett, Bart Massey et al"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xcb.freedesktop.org/dist/$PKG_NAME-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f $PREFIX/share/pkgconfig/xcb-proto.pc ]; then
		cd repo
		./configure --prefix=$PREFIX
		make -j$(nproc) install
	fi
}

build_header

build

wont_package