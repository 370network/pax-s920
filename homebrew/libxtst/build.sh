#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libXtst"
PKG_DESC="X11 client library that provides the API for the XTEST and RECORD extensions in the X Window System"
PKG_VER="1.2.6"
PKG_AUTHOR="X.Org Foundation"

build_dependency "libxi"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/$PKG_NAME-$PKG_VER.tar.gz
	fi
	
	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f repo/src/.libs/libXtst.so.6.1.0 ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

_packaging() {
	cp $PREFIX/lib/libXtst.so $PKG_LIB_DIR/
	cp $PREFIX/lib/libXtst.so.6 $PKG_LIB_DIR/
	cp $PREFIX/lib/libXtst.so.6.1.0 $PKG_LIB_DIR/
}

build_header

build

package_init

packaging

package_create
