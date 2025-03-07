#!/bin/bash
# build.sh - Morc @ 370network
source $PREFIX/../homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build GPMD85emu="
echo "=================="

git clone https://github.com/mborik/GPMD85Emulator repo

if [ ! -f repo/GPMD85emu ]; then
	echo "GPMD85Emulator build process!"
	cd repo
	autoreconf -vfi
	export libSDL_CFLAGS=-I$PREFIX/include/SDL2
	export libSDL_LIBS="-L$PREFIX/lib -lSDL2"
	./configure --host=$HOST --prefix=$PREFIX
	make -j$(nproc)
	cd $BUILD_FOLDER
fi

build_check "GPMD85emu" "gpmd85emu"
