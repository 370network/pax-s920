#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "build pthread-stub"
echo "=================="

if [ ! -f libpthread-stubs.tar.gz ]; then
	curl -o libpthread-stubs.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libpthread-stubs-0.5.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libpthread-stubs.tar.gz
	mv libpthread-stubs-0.5 repo
fi

if [ ! -f $PREFIX/share/pkgconfig/pthread-stubs.pc ]; then
	echo "pthread-stubs build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
