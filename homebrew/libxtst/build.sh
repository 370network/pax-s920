#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build libXtst ="
echo "=================="

echo "Dependency setup!"
cd ../libxi
./build.sh

cd ../libxtst

if [ ! -f libxtst.tar.gz ]; then
	curl -o libxtst.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libXtst-1.2.6.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libxtst.tar.gz
	mv libXtst-1.2.6 repo
fi

if [ ! -f $PREFIX/lib/libXtst.so ]; then
	echo "libXtst build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST --disable-malloc0returnsnull
	make -j$(nproc)
	make install
fi
