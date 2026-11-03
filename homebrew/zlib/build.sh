#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build zlib   ="
echo "=================="

if [ ! -f zlib.tar.gz ]; then
	curl -o zlib.tar.gz -L -O https://zlib.net/zlib-1.3.2.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf zlib.tar.gz
	mv zlib-1.3.2 repo
fi

if [ ! -f repo/libz.so.1.3.2 ]; then
	echo "zlib build process!"
	cd repo
	./configure --prefix=$PREFIX
	make -j$(nproc) AR="$AR" ARFLAGS=rc RANLIB="${RANLIB:-${HOST}-ranlib}"
	make install AR="$AR" ARFLAGS=rc RANLIB="${RANLIB:-${HOST}-ranlib}"
fi
