#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libxcb"
PKG_DESC="Library implementing the X11 Authorization Protocol"
PKG_VER="1.17.0"
PKG_AUTHOR="X.Org Foundation"
PKG_DEPENDS="libXau libXdmcp"

build_dependency "xcb-proto"
build_dependency "libxau"
build_dependency "libxdmcp"
build_dependency "libpthread-stubs"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/$PKG_NAME-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f repo/src/.libs/libxcb.so.1.1.0 ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

_packaging() {
	cp $PREFIX/lib/libxcb.so $PKG_LIB_DIR/
	cp $PREFIX/lib/libxcb.so.1 $PKG_LIB_DIR/
	cp $PREFIX/lib/libxcb.so.1.1.0 $PKG_LIB_DIR/
}

build_header

build

package_init

packaging

package_create
