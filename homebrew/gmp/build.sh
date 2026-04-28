#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=    build gmp   ="
echo "=================="

if [ ! -f gmp.tar.xz ]; then
	curl -o gmp.tar.xz -L -O https://gmplib.org/download/gmp/gmp-6.3.0.tar.xz
fi

if [ ! -d repo ]; then
	tar -xf gmp.tar.xz
	mv gmp-6.3.0 repo
fi

if [ ! -f repo/.libs/libgmp.so.10.5.0 ]; then
	echo "gmp build process!"
	cd repo
	unset CC
	unset CXX
	unset LD
	unset GXX
	unset AS
	unset STRIP
	unset LD_LIBRARY_PATH
	./configure --enable-static --prefix=$PREFIX --host=$HOST_GLIBC
	make -j$(nproc)
	make install
fi
