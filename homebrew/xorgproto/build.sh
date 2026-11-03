#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build xorgproto="
echo "=================="

if [ ! -f xorgproto.tar.gz ]; then
	curl -o xorgproto.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/proto/xorgproto-2026.1.tar.xz
fi

if [ ! -d repo ]; then
	tar -xf xorgproto.tar.gz
	mv xorgproto-2026.1 repo
fi

if [ ! -f $PREFIX/share/pkgconfig/xproto.pc ]; then
	echo "xorgproto build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
