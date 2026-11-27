#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libXi"
PKG_DESC="Extension to the X11 protocol to support input devices other than the core X keyboard and pointer"
PKG_VER="1.8.4"
PKG_AUTHOR="X.Org Foundation"

build_dependency "libxext"
build_dependency "libxfixes"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/$PKG_NAME-$PKG_VER.tar.gz
	fi
	
	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f repo/src/.libs/libXi.so.6.1.0 ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

_packaging() {
	cp $PREFIX/lib/libXi.so $PKG_LIB_DIR/
	cp $PREFIX/lib/libXi.so.6 $PKG_LIB_DIR/
	cp $PREFIX/lib/libXi.so.6.1.0 $PKG_LIB_DIR/
}

build_header

build

package_init

packaging

package_create
