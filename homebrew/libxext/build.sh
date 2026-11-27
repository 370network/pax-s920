#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libXext"
PKG_DESC="Provides an X Window System client interface to several extensions to the X protocol"
PKG_VER="1.3.7"
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

	if [ ! -f repo/src/.libs/libXext.so.6.4.0 ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

_packaging() {
	cp $PREFIX/lib/libXext.so $PKG_LIB_DIR/
	cp $PREFIX/lib/libXext.so.6 $PKG_LIB_DIR/
	cp $PREFIX/lib/libXext.so.6.4.0 $PKG_LIB_DIR/
}

build_header

build

package_init

packaging

package_create
