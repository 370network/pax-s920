#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="LibXUI Font Patch"
PKG_DESC="Patcher that forces antialiasing on all fonts rendered by FreeType using the XUI toolkit. It is recommended to install this patch to internal storage."
PKG_VER="0.1"
PKG_AUTHOR="Morc <morc@370.network>"

_build() {
	if [ ! -f libxui_shim.o ]; then
		make -j$(nproc)
	fi
}

_packaging() {
	cp "libxui.so" "$PKG_LIB_DIR/libxui.so"
}

build_header

build

package_init

packaging

package_create