#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build xtrans ="
echo "=================="

if [ ! -f xtrans.tar.gz ]; then
	curl -o xtrans.tar.gz -L -O https://xorg.freedesktop.org/archive/individual/lib/xtrans-1.6.0.tar.gz
fi

if [ ! -d repo ]; then
	tar -xf xtrans.tar.gz
	mv xtrans-1.6.0 repo
fi

if [ ! -f $PREFIX/share/pkgconfig/xtrans.pc ]; then
	echo "xtrans build process!"
	cd repo
	./configure --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
