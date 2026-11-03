#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build libXdmcp ="
echo "=================="

echo "Dependency setup!"
cd ../xorgproto
./build.sh

cd ../libxdmcp

if [ ! -f libxdmcp.tar.gz ]; then
	curl -o libxdmcp.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libXdmcp-1.1.5.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libxdmcp.tar.gz
	mv libXdmcp-1.1.5 repo
fi

if [ ! -f $PREFIX/lib/libXdmcp.so ]; then
	echo "libXdmcp build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
