#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build ClassiCube="
echo "=================="

git clone https://github.com/ClassiCube/ClassiCube --depth 1 repo

for patch_file in "patches"/*.patch; do
	build_patch $patch_file "repo"
done

if [ ! -f repo/ClassiCube ]; then
	echo "ClassiCube build process!"
	make -j$(nproc) -C repo/ linux BUILD_SDL2=1
fi

build_check "ClassiCube" "ClassiCube/ClassiCube_bin"
