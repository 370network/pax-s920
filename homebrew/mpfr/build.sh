#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build mpfr   ="
echo "=================="

if [ ! -f mpfr.tar.xz ]; then
	curl -o mpfr.tar.xz -L -O https://www.mpfr.org/mpfr-current/mpfr-4.2.2.tar.xz
fi

if [ ! -d repo ]; then
	tar -xf mpfr.tar.xz
	mv mpfr-4.2.2 repo
fi

if [ ! -f repo/src/.libs/libmpfr.so.6.2.2 ]; then
	echo "mpfr build process!"
	cd repo
	unset CC
	unset CXX
	unset LD
	unset GXX
	unset AS
	unset STRIP
	unset LD_LIBRARY_PATH
	./configure --enable-static --with-gmp=$PREFIX --prefix=$PREFIX --host=$HOST_GLIBC CPPFLAGS="-I$PREFIX/include" LDFLAGS="-L$PREFIX/lib"
	make -j$(nproc)
	make install
fi
