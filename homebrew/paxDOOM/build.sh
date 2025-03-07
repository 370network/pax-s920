#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build paxDOOM  ="
echo "=================="

git clone https://github.com/370network/paxDOOM.git repo

export CROSS_COMPILE=$PREFIX/bin/$HOST-

if [ ! -f repo/fbdoom/fbdoom ]; then
	echo "paxDOOM build process!"
	make -j$(nproc) -C repo/fbdoom/
fi

build_check "fbdoom/fbdoom" "paxdoom"
