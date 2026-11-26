#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="ClassiCube"
PKG_DESC="Custom Minecraft Classic compatible client written in C"
PKG_VER="1.3.9"
PKG_AUTHOR="UnknownShadow200 and contributors & Morc @ 370network"
PKG_DEPENDS="SDL2"
PKG_ISAPP=1

build_dependency "SDL"

_build() {
	if [ ! -d repo ]; then
		git clone https://github.com/ClassiCube/ClassiCube --depth 1 repo
	fi

	build_apply_all_patches "repo"

	if [ ! -f repo/ClassiCube ]; then
		make -j$(nproc) -C repo/ linux BUILD_SDL2=1
	fi
}

_packaging() {
	cp icon.png $PKG_APP_DIR
	cp ClassiCube_Loader "$PKG_APP_DIR/$_pkg_name"
	cp "repo/ClassiCube" "$PKG_APP_DIR/ClassiCube_bin"
}

build_header

build

package_init

packaging

package_create
