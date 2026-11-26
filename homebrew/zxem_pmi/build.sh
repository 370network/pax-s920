#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

_build() {
	if [ ! -d repo ]; then
			git clone https://github.com/Sprotex/hax-pax/ --depth 1 repo
	fi

	if [ ! -f repo/makefileupdated ]; then
		cp patches/Makefile repo/Makefile
		echo "done" > repo/makefileupdated
	fi

	if [ ! -f repo/zxemupdated ]; then
		cp patches/zxem.c repo/src/zxem.c
		echo "done" > repo/zxemupdated
	fi

	if [ ! -f repo/zxem ]; then
		make -j$(nproc) -C repo zxem
	fi
}

PKG_NAME="ZX Spectrum Emulator"
PKG_DESC="Custom ZX Spectrum Emulator for ProlinOS"
PKG_VER="1.0.0"
PKG_AUTHOR="Sprotex & ikonko"
PKG_ISAPP=1

build_header

build

package_init

_packaging() {
	cp "repo/zxem" "$PKG_APP_DIR/$_pkg_name.so"
	cp zxem_icon.png "$PKG_APP_DIR/icon.png"
	cp zx48.rom "$PKG_APP_DIR/"
}
packaging

package_create


# PMI APP
PKG_NAME="PMI-80"
PKG_DESC="TESLA PMI-80 Emulator"
PKG_AUTHOR="Sprotex"

build_header

build

package_init

_packaging() {
	cp "repo/pmi" "$PKG_APP_DIR/$_pkg_name.so"
	cp pmi_icon.png "$PKG_APP_DIR/icon.png"
	cp zx48.rom "$PKG_APP_DIR/"
}
packaging

package_create
