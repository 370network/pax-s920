#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build paxman  ="
echo "=================="

if [ ! -f paxman/paxman.so ]; then
	echo "paxman build process!"
	make
	mv paxman.so paxman/paxman.so
fi

