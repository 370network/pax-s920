#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libX11"
PKG_DESC="X Window System protocol client library"
PKG_VER="1.8.14"
PKG_AUTHOR="X.Org Foundation"
PKG_DEPENDS="libxcb"

build_dependency "libxcb"
build_dependency "xorgproto"
build_dependency "xtrans"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/$PKG_NAME-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f repo/src/.libs/libX11.so.6.4.0 ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

_packaging() {
	cp $PREFIX/lib/libX11.so $PKG_LIB_DIR/
	cp $PREFIX/lib/libX11.so.6 $PKG_LIB_DIR/
	cp $PREFIX/lib/libX11.so.6.4.0 $PKG_LIB_DIR/
}

build_header

build

package_init

packaging

package_create
