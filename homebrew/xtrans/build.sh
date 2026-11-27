#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="xtrans"
PKG_DESC="The xorgproto package provides the header files required to build the X Window system, and to allow other applications to build against the installed X Window system."
PKG_VER="1.6.0"
PKG_AUTHOR="X.Org Foundation"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/$PKG_NAME-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f $PREFIX/share/pkgconfig/xtrans.pc ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

build_header

build

wont_package
