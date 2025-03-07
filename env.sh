#!/bin/bash
# pax-build envset
source xcb/bin/activate

export LD_LIBRARY_PATH=$PWD/toolchain/lib
export QEMU_LD_PREFIX=$PWD/toolchain/arm-none-linux-gnueabi/libc/
export PATH=$PWD/toolchain/bin:$PATH
export PREFIX=$PWD/toolchain
export BUILD=x86_64-linux-gnu
export HOST=arm-none-linux-gnueabi
export PAXPATH=$PWD

function retry_cmd() {
	local attempt=0

	while [ $attempt -le 10 ]; do
		eval "$1" && return 0
		exit_code=$?

		if [ $exit_code -eq $2 ]; then
			echo "attempt $attempt failed, retrying in 3 seconds..."
			sleep 3
			((attempt++))
		fi
	done

	echo "failed after 10 attempts..."
	return
}

function paxpush() {
	if [ "$#" -lt 2 ]; then
		echo "usage: paxpush <source file> <pax destination>"
		return
	fi

	if [ -d "$1" ]; then
		find "$1" -type f -print0 | while IFS= read -r -d '' file; do
			relative="${file#$1/}"
			printf "pushing $file to $2/$relative "
			retry_cmd "python3 $PAXPATH/xcb/client.py h push '$file' '$2/$relative'" 1
			sleep 2
		done
	else
		retry_cmd "python3 $PAXPATH/xcb/client.py h push '$1' '$2'" 1
	fi
}

function paxpull() {
	if [ "$#" -lt 2 ]; then
		echo "usage: paxpull <pax source> <local destination>"
		return
	fi

	retry_cmd "python3 $PAXPATH/xcb/client.py h pull '$1' '$2'" 1
}
