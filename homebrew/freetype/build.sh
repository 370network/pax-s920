#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="FreeType"
PKG_DESC="Library used to render text onto bitmaps"
PKG_VER="2.4.8"
PKG_AUTHOR="FreeType contributors"

build_dependency "zlib"

_build() {
	if [ ! -f freetype.tar.gz ]; then
		curl -o freetype.tar.gz -L -O https://download.savannah.gnu.org/releases/freetype/freetype-old/freetype-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf freetype.tar.gz
		mv freetype-$PKG_VER repo
	fi

	if [ ! -f repo/objs/.libs/libfreetype.so.6.8.0 ]; then
		cd repo
		#we need to hack around ancient autoconfigure with a forced build, atleast it works on aarch64 now
		ac_cv_build="$BUILD" ./configure --prefix=$PREFIX --host=$HOST --without-bzip2
		make -j$(nproc) install
	fi
}

build_header

build

wont_package
