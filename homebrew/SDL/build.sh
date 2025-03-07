#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build sdl 1  ="
echo "=================="

git clone https://github.com/370network/SDL-1.2-pax.git repo

if [ ! -f repo/build/.libs/libSDL-1.2.so.0.11.5 ]; then
	echo "SDL1.2 build process!"
	cd repo
	./configure --disable-pulseaudio --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
