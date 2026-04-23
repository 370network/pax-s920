#!/bin/bash
# pax-build envset
# 2nd revision | 2026

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

#script path
if [ "$env_platform" = "linux-gnu" ]; then
	export PAXPATH=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
elif [ "$env_platform" = "apple-darwin" ]; then
	export PAXPATH=${0:A:h}
else
	export PAXPATH=$PWD
fi

source $PAXPATH/xcb/bin/activate

export HOST=arm-unknown-linux-gnueabi
glibc_string=2.13
CC=$HOST$glibc_string-gcc
CXX=$HOST$glibc_string-g++
export CC=$CC
export CXX=$CXX
export AS=$HOST-as
export AR=$HOST-ar
export LD=$HOST-ld
export NM=$HOST-nm
export BUILD=$env_arch-$env_platform
export PATH=$PAXPATH/toolchain/bin:$PATH
export PREFIX=$PAXPATH/toolchain
export LD_LIBRARY_PATH=$PAXPATH/toolchain/lib
export QEMU_LD_PREFIX=$PAXPATH/toolchain/arm-unknown-linux-gnueabi2.13/lib/
export CPPFLAGS="--sysroot=$PAXPATH/toolchain/arm-unknown-linux-gnueabi -I$PAXPATH/toolchain/include"
export CXXFLAGS=$CPPFLAGS
export CFLAGS=$CPPFLAGS
export CROSS_COMPILE_TRIPLET='arm-unknown-linux-gnueabi'
export CROSS_COMPILE_SYSTEM='linux'
export CROSS_COMPILE_ARCHITECTURE='arm'
export CROSS_COMPILE_GLIBCVER='2.13'
export CROSS_COMPILE_SYSROOT=$PAXPATH/toolchain/arm-unknown-linux-gnueabi2.13
export OBGGCC_HOME=$PAXPATH/toolchain/

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

if [ ! -f $PAXPATH/.xcb_config ]; then
	paxreconfigure
else
	source $PAXPATH/.xcb_config
fi