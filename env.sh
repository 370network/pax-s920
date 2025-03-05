#!/bin/bash
# pax-build envset
export LD_LIBRARY_PATH=$PWD/toolchain/lib
export QEMU_LD_PREFIX=$PWD/toolchain/arm-none-linux-gnueabi/libc/
export PATH=$PWD/toolchain/bin:$PATH
export PREFIX=toolchain/$PWD
export BUILD=x86_64-linux-gnu
export HOST=arm-none-linux-gnueabi
