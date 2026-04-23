#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build hax-pax  ="
echo "=================="

git clone https://github.com/Sprotex/hax-pax/ repo

if [ ! -f repo/makefileupdated ]; then
	cp Makefile repo/Makefile
	echo "done" > repo/makefileupdated
fi

if [ ! -f repo/zxem ]; then
	echo "zxem build process!"
	make -j$(nproc) -C repo zxem
fi

if [ ! -f repo/pmi ]; then
        echo "pmi build process!"
        make -j$(nproc) -C repo pmi
fi

if [ ! -f repo/telnet ]; then
        echo "telnet build process!"
        make -j$(nproc) -C repo telnet
fi

build_check "zxem" "zxem"
build_check "pmi" "pmi"
build_check "telnet" "telnet"
