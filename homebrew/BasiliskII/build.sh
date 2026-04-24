#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build BasiliskII="
echo "=================="

git clone https://github.com/kanjitalk755/macemu

if [ ! -d repo ]; then
	cp -r macemu/BasiliskII repo
fi

if [ ! -f repo/build/src/tracker/libmilkytracker.so ]; then
	echo "BasiliskII build process!"

	cd repo/src/Unix
	unset CC
	unset CXX
	unset LD
	unset GXX
	unset AS
	unset STRIP
	unset LD_LIBRARY_PATH
	NO_CONFIGURE=1 ./autogen.sh
	./configure --host=$HOST_GLIBC --prefix=$PREFIX --with-sdl1 CPPFLAGS="-I$PREFIX/include -I$PREFIX/include/SDL" LDFLAGS="-L$PREFIX/lib" LIBS="-lSDL"
	make -j$(nproc)
fi
