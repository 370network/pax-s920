#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build libXfont2 ="
echo "=================="

echo "Dependency setup!"
cd ../freetype
./build.sh

cd ../libfontenc
./build.sh

cd ../xtrans
./build.sh

cd ../xorgproto
./build.sh

cd ../libxfont2

if [ ! -f libxfont2.tar.gz ]; then
	curl -o libxfont2.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libXfont2-2.0.9.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libxfont2.tar.gz
	mv libXfont2-2.0.9 repo
fi

if [ ! -f $PREFIX/lib/pkgconfig/xfont2.pc ]; then
	echo "libXfont2 build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
