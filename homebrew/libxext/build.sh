#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build libXext ="
echo "=================="

echo "Dependency setup!"
cd ../libx11
./build.sh

cd ../libxext

if [ ! -f libxext.tar.gz ]; then
	curl -o libxext.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libXext-1.3.7.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libxext.tar.gz
	mv libXext-1.3.7 repo
fi

if [ ! -f $PREFIX/lib/libXext.so ]; then
	echo "libXext build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST --disable-malloc0returnsnull
	make -j$(nproc)
	make install
fi
