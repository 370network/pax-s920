#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="mpfr"
PKG_DESC="C library for multiple-precision floating-point computations with correct rounding"
PKG_VER="4.2.2"
PKG_AUTHOR="GNU Project"

_build() {
	if [ ! -f mpfr.tar.xz ]; then
		curl -o mpfr.tar.xz -L -O https://www.mpfr.org/mpfr-current/mpfr-$PKG_VER.tar.xz
	fi

	if [ ! -d repo ]; then
		tar -xf mpfr.tar.xz
		mv mpfr-$PKG_VER repo
	fi

	if [ ! -f repo/src/.libs/libmpfr.so.6.2.2 ]; then
		cd repo
		unset CC CXX LD GXX AS STRIP LD_LIBRARY_PATH
		./configure --enable-static --with-gmp=$PREFIX --prefix=$PREFIX --host=$HOST_GLIBC
		make -j$(nproc) install
	fi
}

build_header

build

package_init

_packaging() {
	cp -P $PREFIX/lib/libmpfr.so.6.2.2 $PKG_LIB_DIR
	cp -P $PREFIX/lib/libmpfr.so.6 $PKG_LIB_DIR
	cp -P $PREFIX/lib/libmpfr.so $PKG_LIB_DIR
}

packaging

package_create

