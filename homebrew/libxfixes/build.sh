#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build libXfixes ="
echo "=================="

echo "Dependency setup!"
cd ../libx11
./build.sh

cd ../libxfixes

if [ ! -f libxfixes.tar.gz ]; then
	curl -o libxfixes.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libXfixes-6.0.2.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libxfixes.tar.gz
	mv libXfixes-6.0.2 repo
fi

if [ ! -f $PREFIX/lib/libXfixes.so ]; then
	echo "libXfixes build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST --disable-malloc0returnsnull
	make -j$(nproc)
	make install
fi
