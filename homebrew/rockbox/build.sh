#!/bin/bash
# build.sh - Morc @ 370network
source $PREFIX/../homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build rockbox  ="
echo "=================="

if [ ! -f repo.zip ]; then
	wget https://github.com/Rockbox/rockbox/archive/d13029ebdd0ed9430977097431d292081d78f431.zip -O repo.zip
fi

if [ ! -d repo ]; then
	unzip repo.zip
	mv rockbox-d13029ebdd0ed9430977097431d292081d78f431 repo
fi

for patch_file in "patches"/*.patch; do
	build_patch $patch_file "repo"
done

if [ ! -f repo/build/rockboxui ]; then
	echo "rockbox build process!"
	cd repo
	mkdir build
	cd build
	export CROSS_COMPILE=$PREFIX/bin/$HOST-
	../tools/configure --target=64 --type=S --ram=32 --prefix=$PREFIX
	sed -i 's|^export GCCOPTS=|export GCCOPTS=-fPIC -shared -nostartfiles -march=armv6t2 -mtune=arm1136j-s -ftree-vectorize -funroll-loops -fomit-frame-pointer -ffast-math -s -fno-strict-aliasing |' Makefile
	sed -i 's|^export LDOPTS=|export LDOPTS=-shared |' Makefile
	make -j$(nproc)
	make install
	cd $BUILD_FOLDER
fi

build_check "build/rockboxui" "rockbox"
build_dir_check "$PREFIX/.rockbox" ".rockbox"
