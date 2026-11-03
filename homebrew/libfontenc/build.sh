#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build fontenc  ="
echo "=================="

echo "Dependency setup!"
cd ../zlib
./build.sh

cd ../libfontenc

if [ ! -f libfontenc.tar.gz ]; then
	curl -o libfontenc.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libfontenc-1.1.9.tar.xz
fi

if [ ! -d repo ]; then
	tar -xf libfontenc.tar.gz
	mv libfontenc-1.1.9 repo
fi

if [ ! -f $PREFIX/lib/pkgconfig/fontenc.pc ]; then
	echo "libfontenc build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
