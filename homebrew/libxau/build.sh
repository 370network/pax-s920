#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build libXau ="
echo "=================="

echo "Dependency setup!"
cd ../xorgproto
./build.sh

cd ../libxau

if [ ! -f libxau.tar.gz ]; then
	curl -o libxau.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libXau-1.0.12.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libxau.tar.gz
	mv libXau-1.0.12 repo
fi

if [ ! -f $PREFIX/lib/libXau.so ]; then
	echo "libXau build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
