#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh
 
echo "=================="
echo "370network paxdevs"
echo "=build libXrender="
echo "=================="
 
echo "Dependency setup!"
cd ../libx11
./build.sh
 
cd ../libxrender
 
if [ ! -f libxrender.tar.gz ]; then
	curl -o libxrender.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libXrender-0.9.13.tar.gz
fi
 
if [ ! -d repo ]; then
	tar -xf libxrender.tar.gz
	mv libXrender-0.9.13 repo
fi
 
if [ ! -f $PREFIX/lib/libXrender.so ]; then
	echo "libXrender build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST --disable-malloc0returnsnull CFLAGS="$CFLAGS -std=gnu17" CPPFLAGS="$CPPFLAGS -std=gnu17"
	make -j$(nproc)
	make install
fi
 
