#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build ClassiCube="
echo "=================="

git clone https://github.com/ClassiCube/ClassiCube repo

for patch_file in "patches"/*.patch; do
	build_patch $patch_file "repo"
done

if [ ! -f repo/ClassiCube ]; then
	echo "ClassiCube build process!"
	$CC repo/src/*.c -o repo/ClassiCube -I$PREFIX/include/ -L$PREFIX/lib -lSDL2 -lm -ldl -lpthread -lrt
fi

#build_check "ClassiCube" "ClassiCube"
