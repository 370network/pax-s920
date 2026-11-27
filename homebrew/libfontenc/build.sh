#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libfontenc"
PKG_DESC="The libfontenc library is used by the Xorg server and other X font tools for handling fonts with different character set encodings."
PKG_VER="1.1.10"
PKG_AUTHOR="X.Org Foundation"

build_dependency "zlib"
build_dependency "xorgproto"

_build() {
	if [ ! -f $PKG_NAME.tar.gz ]; then
		curl -o $PKG_NAME.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/$PKG_NAME-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf $PKG_NAME.tar.gz
		mv $PKG_NAME-$PKG_VER repo
	fi

	if [ ! -f repo/src/.libs/libfontenc.so.1.0.0 ]; then
		cd repo
		./configure --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

_packaging() {
	cp $PREFIX/lib/libfontenc.so $PKG_LIB_DIR/
	cp $PREFIX/lib/libfontenc.so.1.0.0 $PKG_LIB_DIR/
	cp $PREFIX/lib/libfontenc.so.1 $PKG_LIB_DIR/
}

build_header

build

package_init

packaging

package_create