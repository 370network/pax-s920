#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="xorgproto"
PKG_DESC="The xorgproto package provides the header files required to build the X Window system, and to allow other applications to build against the installed X Window system."
PKG_VER="2026.1"
PKG_AUTHOR="X.Org Foundation"

_build() {
	if [ ! -f $PKG_NAME.tar.xz ]; then
		curl -o $PKG_NAME.tar.xz -L -O https://xorg.freedesktop.org/archive/individual/proto/$PKG_NAME-$PKG_VER.tar.xz
	fi

	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.xz
		mv $PKG_NAME-$PKG_VER repo
	fi
	
	if [ ! -f $PREFIX/share/pkgconfig/xproto.pc ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

build_header

build

wont_package
