#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build xcb-proto="
echo "=================="

if [ ! -f xcb-proto.tar.gz ]; then
	curl -o xcb-proto.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/proto/xcb-proto-1.17.0.tar.xz
fi

if [ ! -d repo ]; then
	tar -xf xcb-proto.tar.gz
	mv xcb-proto-1.17.0 repo
fi

if [ ! -f $PREFIX/share/pkgconfig/xcb-proto.pc ]; then
	echo "xcb-proto build process!"
	cd repo
	./configure --prefix=$PREFIX
	make -j$(nproc)
	make install
fi
