#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build springboard="
echo "=================="

git clone https://github.com/370network/pax-springboard.git repo

if [ ! -f repo/launcher.so ]; then
	make clean -C repo
	make -j$(nproc) -C repo
fi

build_check "launcher.so" "launcher"
