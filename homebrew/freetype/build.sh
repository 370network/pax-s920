#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build freetype ="
echo "=================="

echo "Dependency setup!"
cd ../zlib
./build.sh

cd ../freetype

if [ ! -f freetype.tar.xz ]; then
	curl -o freetype.tar.xz -L -O https://downloads.sourceforge.net/freetype/freetype-2.13.3.tar.xz
fi

if [ ! -d repo ]; then
	tar -xf freetype.tar.xz
	mv freetype-2.13.3 repo
fi

if [ ! -f $PREFIX/lib/pkgconfig/freetype2.pc ]; then
	echo "freetype build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST --without-harfbuzz --without-png --without-bzip2 --without-brotli
	make -j$(nproc)
	make install
fi
