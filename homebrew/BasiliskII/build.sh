#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build BasiliskII="
echo "=================="

if [ $env_platform != "linux*" ]; then
	echo "You are currently building on $env_platform, this environment is UNSUPPORTED!"
	echo "BasiliskII compilation process currently depends on a Linux environment with qemu-binfmt!"
	echo "If you want to compile BasiliskII, you have to do so elsewhere. Sorry :("
	exit
fi

git clone https://github.com/kanjitalk755/macemu repo-macemu

if [ ! -d repo ]; then
	cp -r repo-macemu/BasiliskII repo
fi

if [ ! -f repo/src/Unix/BasiliskII ]; then
	echo "BasiliskII build process!"

	cd repo/src/Unix
	unset CC
	unset CXX
	unset LD
	unset GXX
	unset AS
	unset STRIP
	unset LD_LIBRARY_PATH
	unset CPPFLAGS
	unset LDFLAGS
	unset LIBS
	NO_CONFIGURE=1 ./autogen.sh
	./configure --host=$HOST_GLIBC --prefix=$PREFIX --with-sdl1 BII_CROSS_HAVE_EXTENDED_SIGNALS=yes CPPFLAGS="-I$PREFIX/include -I$PREFIX/include/SDL" LDFLAGS="-L$PREFIX/lib" LIBS="-lSDL"
	make -j$(nproc)
fi
