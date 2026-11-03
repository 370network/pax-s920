#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build libxcb  ="
echo "=================="

echo "Dependency setup!"
cd ../xcb-proto
./build.sh

cd ../libxau
./build.sh

cd ../libxdmcp
./build.sh

cd ../libpthread-stubs
./build.sh

cd ../libxcb

if [ ! -f libxcb.tar.gz ]; then
	curl -o libxcb.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/libxcb-1.17.0.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf libxcb.tar.gz
	mv libxcb-1.17.0 repo
fi

if [ ! -f $PREFIX/lib/libxcb.so ]; then
	echo "libxcb build process!"
	cd repo
	PYTHON=python3 ./configure --prefix=$PREFIX --host=$HOST --disable-build-docs
	make -j$(nproc)
	make install
fi
