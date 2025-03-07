#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

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
	../tools/configure --target=64 --type=S --prefix=$PREFIX
	sed -i 's| -g | -g0 -fPIC -shared -nostartfiles |' Makefile
	sed -i 's|^export SHARED_CFLAGS=|export SHARED_CFLAGS=-g0 |' Makefile
	sed -i 's|^export LDOPTS=|export LDOPTS=-shared |' Makefile
	sed -i 's|^export MEMORYSIZE=64|export MEMORYSIZE=32|' Makefile
	make -j$(nproc)
	find "." -type f -print0 | while IFS= read -r -d '' file; do
		$HOST-strip $file
	done
	make install
	cd $BUILD_FOLDER
fi

build_check "build/rockboxui" "rockbox"
build_dir_check "$PREFIX/.rockbox" ".rockbox"
