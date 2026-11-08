#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build BasiliskII="
echo "=================="

if [[ $env_platform != "linux-gnu" ]] then
	echo "You are currently building on $env_platform, this environment is UNSUPPORTED!"
	echo "BasiliskII compilation process currently depends on a glibc Linux environment for obscure reasons!"
	echo "If you want to compile BasiliskII, you have to do so elsewhere. Sorry :("
	exit
fi

if [[ $(getconf PAGESIZE) != 4096 ]]; then
        echo "You are currently building on a platform that has a $(getconf PAGESIZE) byte kernel page size!"
        echo "BasiliskII compilation process currently depends on a platform with 4096 bytes!"
        echo "If you want to compile BasiliskII, you have to do so elsewhere. Sorry :("
        exit
fi

if [[ $(command -v qemu-arm) != "" ]] || [[ $(command -v qemu-system-arm) != "" ]] then
        echo "You are missing the qemu-user package!"
        echo "BasiliskII compilation process currently depends on qemu-user (qemu-arm/qemu-system-arm) to work.!"
        echo "If you want to compile BasiliskII, you have to do so elsewhere. Sorry :("
        exit
fi

echo "Dependency setup!"
cd ../gmp
./build.sh

cd ../mpfr
./build.sh

cd ../SDL
./build.sh

cd ../BasiliskII

git clone https://github.com/kanjitalk755/macemu repo-macemu

for patch_file in "patches"/*.patch; do
	build_patch $patch_file "repo-macemu"
done

if [ ! -d repo ]; then
	cp -r repo-macemu/BasiliskII repo
fi

if [ ! -f repo/src/Unix/Makefile ]; then
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
	./configure --host=$HOST_GLIBC --prefix=$PREFIX --with-sdl1 BII_CROSS_HAVE_EXTENDED_SIGNALS=yes CPPFLAGS="-I$PREFIX/include" LDFLAGS="-L$PREFIX/lib" sdl_LIBS="-lSDL" LIBS="-lSDL" sdl_CFLAGS="-I$PREFIX/include/SDL"
fi

if [ ! -f repo/src/Unix/BasiliskII ]; then
	echo "BasiliskII build process!"
	make -j$(nproc)
fi

mv repo/src/Unix/BasiliskII BasiliskII/BasiliskII_bin
