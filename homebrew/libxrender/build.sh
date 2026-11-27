#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libXrender"
PKG_DESC="Client-side C library for the X Rendering Extension (Render) in the X Window System"
PKG_VER="0.9.13"
PKG_AUTHOR="X.Org Foundation"

build_dependency "libx11"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/$PKG_NAME-$PKG_VER.tar.gz
	fi
	
	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f repo/src/.libs/libXrender.so.1.3.0 ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

_packaging() {
	cp $PREFIX/lib/libXrender.so $PKG_LIB_DIR/
	cp $PREFIX/lib/libXrender.so.1 $PKG_LIB_DIR/
	cp $PREFIX/lib/libXrender.so.1.3.0 $PKG_LIB_DIR/
}

build_header

build

package_init

packaging

package_create
