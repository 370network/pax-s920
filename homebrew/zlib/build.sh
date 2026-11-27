#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="zlib"
PKG_DESC="A free, general-purpose, legally unencumbered lossless data-compression library"
PKG_VER="1.2.7"
PKG_AUTHOR="Jean-Loup Gailly, Mark Adler et al"

_build() {
	if [ ! -f zlib.tar.gz ]; then
		curl -o zlib.tar.gz -L -O https://zlib.net/fossils/zlib-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf zlib.tar.gz
		mv zlib-$PKG_VER repo
	fi

	if [ ! -f repo/libz.so.$PKG_VER ]; then
		cd repo
		./configure --prefix=$PREFIX
		make -j$(nproc) install AR="$AR" ARFLAGS=rc RANLIB="${HOST}-ranlib"
	fi
}

build_header

build

wont_package
