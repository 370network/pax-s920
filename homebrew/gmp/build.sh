#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="gmp"
PKG_DESC="Free library for arbitrary precision arithmetic"
PKG_VER="6.3.0"
PKG_AUTHOR="Free Software Foundation"

_build() {
	if [ ! -f gmp.tar.xz ]; then
		curl -o gmp.tar.xz -L -O https://gmplib.org/download/gmp/gmp-$PKG_VER.tar.xz
	fi

	if [ ! -d repo ]; then
		tar -xf gmp.tar.xz
		mv gmp-$PKG_VER repo
	fi

	if [ ! -f repo/.libs/libgmp.so.10.5.0 ]; then
		cd repo
		unset CC CXX LD GXX AS STRIP LD_LIBRARY_PATH
		./configure --enable-static --prefix=$PREFIX --host=$HOST_GLIBC
		make -j$(nproc) install
	fi
}

build_header

build

package_init

_packaging() {
	cp -P $PREFIX/lib/libgmp.so.10.5.0 $PKG_LIB_DIR
	cp -P $PREFIX/lib/libgmp.so.10 $PKG_LIB_DIR
	cp -P $PREFIX/lib/libgmp.so $PKG_LIB_DIR
}

packaging

package_create