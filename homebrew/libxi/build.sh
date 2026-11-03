#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build libXi   ="
echo "=================="

echo "Dependency setup!"
cd ../libxext
./build.sh

cd ../libxfixes
./build.sh

cd ../libxi

if [ ! -f libxi.tar.gz ]; then
	curl -o libxi.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libXi-1.8.4.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libxi.tar.gz
	mv libXi-1.8.4 repo
fi

if [ ! -f $PREFIX/lib/libXi.so ]; then
	echo "libXi build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST --disable-malloc0returnsnull
	make -j$(nproc)
	make install
fi
