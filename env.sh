#!/bin/bash
# pax-build envset
export LD_LIBRARY_PATH=$PWD/toolchain/lib
export QEMU_LD_PREFIX=$PWD/toolchain/arm-none-linux-gnueabi/libc/
export PATH=$PWD/toolchain/bin:$PATH
export PREFIX=$PWD/toolchain
export BUILD=x86_64-linux-gnu
export HOST=arm-none-linux-gnueabi

function paxpush() {
 	 python3 xcb/client.py h push $1 $2
}

function paxpull() {
 	 python3 xcb/client.py h pull $1 $2
}
