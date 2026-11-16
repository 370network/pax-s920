#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build openssl ="
echo "=================="

if [ ! -f openssl.tar.gz ]; then
	curl -o openssl.tar.gz -L -O https://github.com/openssl/openssl/releases/download/OpenSSL_1_0_1u/openssl-1.0.1u.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf openssl.tar.gz
	mv openssl-1.0.1u repo
fi

if [ ! -f repo/libssl.so.1.0.0 ]; then
	echo "openssl build process!"
	cd repo
	./Configure linux-armv4 --prefix=$PREFIX shared threads no-zlib
	make -j$(nproc)
	make install
fi
