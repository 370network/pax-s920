#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="xfonts-base"
PKG_DESC="Minimal set of fonts for TinyX based on the Debian archive"
PKG_VER="1.0.5"
PKG_AUTHOR="Debian Project & X.Org Foundation"

_build() {
	if [ ! -f ${PKG_NAME}_${PKG_VER}.deb ]; then
		curl -o ${PKG_NAME}_${PKG_VER}.deb -L -O https://deb.debian.org/debian/pool/main/x/xfonts-base/${PKG_NAME}_${PKG_VER}_all.deb
	fi

	if [ ! -d repo ]; then
		bsdtar -xf ${PKG_NAME}_${PKG_VER}.deb
		bsdtar -xf data.tar.xz
		rm -rf debian-binary etc ${PKG_NAME}_${PKG_VER}.deb control.tar.xz
		mv usr repo

		mkdir repo/misc
		cd repo/share/fonts/X11/misc
		mv cursor.pcf.gz 6x13.pcf.gz 6x13B.pcf.gz 5x8.pcf.gz ../../../../misc
		#mkfontdir ../../../../misc we will hardcode them for now, less packages to install for platforms
	fi

}

_packaging() {
	mkdir -p $PKG_APPS_DIR/TinyX/fonts/
	cp -r "repo/misc" "$PKG_APPS_DIR/TinyX/fonts/"
	cp "fonts.dir" "$PKG_APPS_DIR/TinyX/fonts/misc/"
	cp "fonts.alias" "$PKG_APPS_DIR/TinyX/fonts/misc/"
}

build_header

build

package_init

packaging

package_create
