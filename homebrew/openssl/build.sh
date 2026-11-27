#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="OpenSSL"
PKG_DESC="Robust, commercial-grade, full-featured toolkit for general-purpose cryptography and secure communication"
PKG_VER="1.0.1u"
PKG_AUTHOR="The OpenSSL Project"

_build() {
	if [ ! -f openssl.tar.gz ]; then
		curl -o openssl.tar.gz -L -O https://github.com/openssl/openssl/releases/download/OpenSSL_1_0_1u/openssl-$PKG_VER.tar.gz
	fi

	if [ ! -d repo ]; then
		tar -xf openssl.tar.gz
		mv openssl-$PKG_VER repo
	fi

	if [ ! -f repo/libssl.so.1.0.0 ]; then
		cd repo
		./Configure linux-armv4 --prefix=$PREFIX shared threads no-zlib
		make -j$(nproc)
		make -j$(nproc) install_sw 
	fi
}

build_header

build

wont_package
