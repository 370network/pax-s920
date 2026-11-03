#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build libX11  ="
echo "=================="

echo "Dependency setup!"
cd ../libxcb
./build.sh

cd ../xorgproto
./build.sh

cd ../xtrans
./build.sh

cd ../libx11

if [ ! -f libx11.tar.gz ]; then
	curl -o libx11.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libX11-1.8.14.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libx11.tar.gz
	mv libX11-1.8.14 repo
fi

if [ ! -f $PREFIX/lib/libX11.so ]; then
	echo "libX11 build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST --disable-malloc0returnsnull CFLAGS="$CFLAGS -std=gnu17" CPPFLAGS="$CPPFLAGS -std=gnu17"
	make -j$(nproc)
	make install
fi
