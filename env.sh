#!/usr/bin/env bash
# pax-build envset
# 2nd revision | 2026

#script path
env_tempplat=$(uname -s)

#we have to use POSIX case style instead of regular *sh due to various interpreters
case "$env_tempplat" in
  Linux*)
    export PAXPATH=$(CDPATH= cd -- "$(dirname -- "$0")" 2>/dev/null && pwd)
    ;;
  Darwin)
    export PAXPATH=${0:A:h}
    ;;
  *)
    export PAXPATH=$PWD
    ;;
esac
#get platform
eval "$(bash $PAXPATH/platform.sh)"

#load xcb client
case "$env_distro" in
  *nixos*)
    env_xcb="$(nix path-info "$PAXPATH/xcb/")/bin/paxclient"
    ;;
  *)
    env_xcb="python3 $PAXPATH/xcb/src/main.py"
    source $PAXPATH/xcb/bin/activate
    ;;
esac

export HOST=arm-unknown-linux-gnueabi
export HOST_GLIBC=arm-unknown-linux-gnueabi2.13
CC=$HOST_GLIBC-gcc
CXX=$HOST_GLIBC-g++
export CC=$CC
export CXX=$CXX
export AS=$HOST-as
export AR=$HOST-ar
export LD=$HOST-ld
export NM=$HOST-nm
export STRIP=$HOST-strip
export BUILD=$env_arch-$env_platform
export TOOLCHAIN=$PAXPATH/toolchain
export PREFIX=$TOOLCHAIN/paxroot
export PATH=$TOOLCHAIN/bin:$PATH
export LD_LIBRARY_PATH=$TOOLCHAIN/lib
export QEMU_LD_PREFIX=$TOOLCHAIN/arm-unknown-linux-gnueabi2.13/
export CPPFLAGS="--sysroot=$TOOLCHAIN/arm-unknown-linux-gnueabi -I$PREFIX/include"
export LDFLAGS="-L$PREFIX/lib"
export CXXFLAGS=$CPPFLAGS
export CFLAGS=$CPPFLAGS
export CROSS_COMPILE_TRIPLET='arm-unknown-linux-gnueabi'
export CROSS_COMPILE_SYSTEM='linux'
export CROSS_COMPILE_ARCHITECTURE='arm'
export CROSS_COMPILE_GLIBCVER='2.13'
export CROSS_COMPILE_SYSROOT=$TOOLCHAIN/arm-unknown-linux-gnueabi2.13

function reloadxcb() {
	source $PAXPATH/.xcb_config
	
	if [[ "${env_distro,,}" = *"nixos"* ]]; then
		if [[ "${PAX_CLIENT_IP}" = "" ]]; then
			env_xcb="nix run $PAXPATH/xcb/ -- -s $PAX_CLIENT_SERIAL"
		else
			env_xcb="nix run $PAXPATH/xcb/ -- -s $PAX_CLIENT_IP"
		fi
	fi
}

function paxreconfigure() {
	printf "[*] Enter device address ({IP|hostname}+port or the whole /dev/ path): "
	read addr
	
	if [[ "$addr" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ || "$addr" = *:5555 ]]; then
	    echo "export PAX_CLIENT_IP=\"$addr\"" > .xcb_config
	else
	    echo "export PAX_CLIENT_SERIAL=\"$addr\"" > .xcb_config
	fi
	
	reloadxcb
}

function paxpush() {
	if [ "$#" -lt 2 ]; then
		echo "usage: paxpush <local source 1> <local source 2> .. <device destination>"
		return
	fi

	$env_xcb push $@
}

function paxpull() {
	if [ "$#" -lt 1 ]; then
		echo "usage: paxpull <pax source> [optional: local destination]"
		return
	fi

	$env_xcb pull $1 $2
}

function paxls() {
	if [ "$#" -lt 1 ]; then
		echo "usage: paxls <pax directory>"
		return
	fi

	$env_xcb ls $1
}

function paxdeployssh() {
        local pubkey_file="$1"
        local tmp_auth="authorized_keys"

        $env_xcb pull /data/app/MAINAPP/.ssh/authorized_keys

        if [ ! -f "$tmp_auth" ]; then
                touch "$tmp_auth"
        fi

        if [ -n "$pubkey_file" ]; then
                if [ ! -f "$pubkey_file" ]; then
                        echo "error: file not found: $pubkey_file"
                        return 1
                fi

                cat "$pubkey_file" >> "$tmp_auth"
                echo "appended: $pubkey_file"
        else
                local found=0
                local f

                for f in "$HOME"/.ssh/*.pub; do
                        if [ ! -e "$f" ]; then
                                continue
                        fi

                        cat "$f" >> "$tmp_auth"
                        echo "appended: $f"
                        found=1
                done

                if [ "$found" -eq 0 ]; then
                        echo "error: no public key file found in ~/.ssh/"
                        rm "$tmp_auth"
                        return 1
                fi
        fi

        $env_xcb push "$tmp_auth" /data/app/MAINAPP/.ssh/authorized_keys
        rm "$tmp_auth"
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

	$env_xcb dump $1 $2
}

if [ ! -f $PAXPATH/.xcb_config ]; then
	paxreconfigure
else
	reloadxcb
fi
