#!/bin/bash
# pax-build envset
# 2nd revision | 2026

source xcb/bin/activate

export env_platform=$(uname)
if [ "$env_platform" = "Linux" ]; then
	env_platform="linux-gnu"
elif [ "$env_platform" = "Darwin" ]; then
	env_platform="apple-darwin"
fi

export env_arch=$(uname -m)
if [ "$env_arch" = "arm64" ]; then
	env_arch="aarch64"
fi

export LD_LIBRARY_PATH=$PWD/toolchain/lib
export QEMU_LD_PREFIX=$PWD/toolchain/arm-unknown-linux-gnueabi2.13/lib/
export PATH=$PWD/toolchain/bin:$PATH
export PREFIX=$PWD/toolchain
export BUILD=$env_arch-$env_platform
export HOST=arm-unknown-linux-gnueabi2.13
export PAXPATH=$PWD

function paxreconfigure() {
	printf "[*] Enter device address ({IP|hostname}+port or the whole /dev/ path): "
	read addr
	
	if [[ "$addr" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ || "$addr" = *:5555 ]]; then
	    echo "export PAX_CLIENT_IP=\"$addr\"" > .xcb_config
	else
	    echo "export PAX_CLIENT_SERIAL=\"$addr\"" > .xcb_config
	fi
}

function paxpush() {
	if [ "$#" -lt 2 ]; then
		echo "usage: paxpush <local source 1> <local source 2> .. <device destination>"
		return
	fi

	python3 $PAXPATH/xcb/src/main.py push $@
}

function paxpull() {
	if [ "$#" -lt 1 ]; then
		echo "usage: paxpull <pax source> [optional: local destination]"
		return
	fi

	python3 $PAXPATH/xcb/src/main.py pull $1 $2
}

function paxls() {
	if [ "$#" -lt 1 ]; then
		echo "usage: paxls <pax directory>"
		return
	fi

	python3 $PAXPATH/xcb/src/main.py ls $1
}

function paxdump() {
	if [ "$#" -lt 1 ]; then
		echo "usage: paxdump <dump name> [optional: device path to dump]"
		echo "The resulting dump will be stored in dumps/<dump name>/ inside the working directory"
		return
	fi

	if [ ! -d "$PWD/dumps" ]; then
		mkdir "$PWD/dumps"
		[ "$?" -ne 0 ] && return
	fi

	python3 $PAXPATH/xcb/src/main.py dump $1 $2
}

if [ ! -f .xcb_config ]; then
	paxreconfigure
else
	source .xcb_config
fi