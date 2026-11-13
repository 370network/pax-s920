#!/usr/bin/env bash
# pax-build envset
# 2nd revision | 2026

#script path for shells that give us the path
if [ -n "${BASH_VERSION-}" ]; then
	PAXPATH=${BASH_SOURCE}
elif [ -n "${ZSH_VERSION-}" ]; then
	eval 'PAXPATH=${(%):-%x}'
fi

#evaluate to a folder
if [ -n "${PAXPATH:-}" ] && [ -f "$PAXPATH" ]; then
	PAXPATH=$(CDPATH= cd -P "$(dirname "$PAXPATH")" 2>/dev/null && pwd -P) || return 1
fi

#check if saner shells assigned the folder
if [ -z "${PAXPATH:-}" ]; then
	if [ ! -f ./env.sh ]; then #ash on postmarketOS doesn't get it
		echo "[!] Please traverse into the folder with toolchain." >&2
		return 1
	fi

	#use current pwd because env.sh exists in the current folder 
	PAXPATH=$(pwd -P) || return 1
fi
export PAXPATH

#get platform
eval "$(bash $PAXPATH/platform.sh)"

#load xcb client
case "$env_distro" in
  *nixos*)
	env_xcb="$(nix path-info "$PAXPATH/xcb/")/bin/paxclient"
	env_xcb_script_path=""
	;;
  *)
	. $PAXPATH/xcb/bin/activate
	env_xcb="python3"
	env_xcb_script_path="$PAXPATH/xcb/src/main.py"
	;;
esac
env_xcb_args=""

export HOST=arm-unknown-linux-gnueabi
export HOST_GLIBC=${HOST}2.13
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
export QEMU_LD_PREFIX=$TOOLCHAIN/$HOST_GLIBC/
export CPPFLAGS="--sysroot=$TOOLCHAIN/$HOST_GLIBC/ -I$PREFIX/include"
export CFLAGS="$CPPFLAGS -mcpu=arm1136j-s -O2 -fno-strict-aliasing -funroll-all-loops"
export CXXFLAGS=$CFLAGS
export LDFLAGS="-L$PREFIX/lib -s"
export CROSS_COMPILE_TRIPLET=$HOST
export CROSS_COMPILE_SYSTEM='linux'
export CROSS_COMPILE_ARCHITECTURE='arm'
export CROSS_COMPILE_GLIBCVER='2.13'
export CROSS_COMPILE_SYSROOT=$TOOLCHAIN/$HOST_GLIBC



paxreconfigure() {
	printf "[*] Enter device address ({IP|hostname}+port or the whole /dev/ path): "
	read addr
	
	if [[ "$addr" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ || "$addr" = *:5555 ]]; then
		echo "export PAX_CLIENT_IP=\"$addr\"" > $PAXPATH/.xcb_config
	else
		echo "export PAX_CLIENT_SERIAL=\"$addr\"" > $PAXPATH/.xcb_config
	fi
	
	. $PAXPATH/.xcb_config
}

paxpush() {
	if [ "$#" -lt 2 ]; then
		echo "usage: paxpush <local source 1> <local source 2> .. <device destination>"
		return
	fi

	$env_xcb $env_xcb_script_path $env_xcb_args push $@
}

paxpull() {
	if [ "$#" -lt 1 ]; then
		echo "usage: paxpull <pax source> [optional: local destination]"
		return
	fi

	$env_xcb $env_xcb_script_path $env_xcb_args pull $1 $2
}

paxls() {
	if [ "$#" -lt 1 ]; then
		echo "usage: paxls <pax directory>"
		return
	fi

	$env_xcb $env_xcb_script_path $env_xcb_args ls $1
}

paxdeployssh() {
		local pubkey_file="$1"
		local tmp_auth="authorized_keys"

		$env_xcb $env_xcb_script_path pull /data/app/MAINAPP/.ssh/authorized_keys

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

		$env_xcb $env_xcb_script_path push "$tmp_auth" /data/app/MAINAPP/.ssh/authorized_keys
		rm "$tmp_auth"
}

paxdump() {
	if [ "$#" -lt 1 ]; then
		echo "usage: paxdump <dump name> [optional: device path to dump]"
		echo "The resulting dump will be stored in dumps/<dump name>/ inside the working directory"
		return
	fi

	if [ ! -d "$PWD/dumps" ]; then
		mkdir "$PWD/dumps"
		[ "$?" -ne 0 ] && return
	fi

	$env_xcb $env_xcb_script_path dump $1 $2
}



_AUTOCOMP_CACHE_DIR=''
_AUTOCOMP_CACHE_DATA=''
_AUTOCOMP_CACHE_TIME=-999999
_pax_autocomplete_prepare() {
	local dir prefix line entry mode candidate output now
	
	env_xcb_args="-t 2000"
	now=$SECONDS
	_AUTOCOMP_MATCHES=''

	if [[ "$1" == */* ]]; then
		dir="${1%/*}"
		prefix="${1%/*}/"
		[[ -n "$dir" ]] || dir="/"
	else
		dir="."
		prefix=""
	fi
	
	#2 second TTL for using the cached data
	if [[ "$_AUTOCOMP_CACHE_DIR" == "$dir" ]] && (( 2 > 0 && now - _AUTOCOMP_CACHE_TIME < 2 )); then
		_AUTOCOMP_LISTING="$_AUTOCOMP_CACHE_DATA"
	else #recaching from scratch
		if output="$(paxls "$dir" 2>/dev/null)"; then
			_AUTOCOMP_CACHE_DIR="$dir"
			_AUTOCOMP_CACHE_DATA="$output"
			_AUTOCOMP_CACHE_TIME=$now
			_AUTOCOMP_LISTING="$output"
		else #no data
			_AUTOCOMP_LISTING=''
			return 1
		fi
	fi

	while IFS= read -r line; do
		#continue if line has Perm:
		[[ "$line" == *" Perm: "* ]] || continue

		entry="${line%% Perm: *}"
		mode="${line#* Perm: }"
		mode="${mode%% *}"

		#skip . and .. for obvious reasons
		if [ -z "${1##*/}" ] && { [ "$entry" = "." ] || [ "$entry" = ".." ]; }; then
			continue
		fi

		candidate="${prefix}${entry}"

		#add slash to folders (this is actually broken with symlinks or mounts?)
		[[ "$mode" == 0o4* ]] && candidate+="/"

		_AUTOCOMP_MATCHES+="${candidate}"$'\n'

	done <<< "$_AUTOCOMP_LISTING"

	env_xcb_args=""
	return 0
}

#install autocomplete
if [[ -n "${ZSH_VERSION-}" ]]; then
	if ! whence -w compdef >/dev/null 2>&1; then
        autoload -Uz compinit
        compinit
    fi
	
	. $PAXPATH/autocomplete_zsh.sh
    
    if command -v compdef >/dev/null 2>&1; then
    	compdef _pax_autocomplete_zsh paxls paxpull paxpush
        #echo "[+] Zsh autocomplete installed"
    fi
elif command -v complete >/dev/null 2>&1; then #bash
	. $PAXPATH/autocomplete_bash.sh
	complete -o default -F _pax_autocomplete_bash paxls paxpull paxpush
    #echo "[+] Bash autocomplete installed"
fi



if [ ! -f $PAXPATH/.xcb_config ]; then
	paxreconfigure
else
	. $PAXPATH/.xcb_config
fi

echo "Welcome to the 370network PAX/ProlinOS build environment!"
