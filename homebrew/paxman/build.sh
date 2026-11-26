#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="PAXMAN"
PKG_DESC="Arcade port of PACMAN"
PKG_VER="1.1.0"
PKG_AUTHOR="Pacbox (Alessandro Scoti), PIE, Dave Chapman and Morc @ 370network"
PKG_ISAPP=1

_build() {
	if [ ! -f src/paxman.so ]; then
		make -j$(nproc)
	fi
}

_packaging() {
	cp icon.png $PKG_APP_DIR
	cp "src/paxman.so" "$PKG_APP_DIR/$_pkg_name.so"
	#cp -r pacman $PKG_APP_DIR/ roms are now part of a separate package, I don't want to distribute them in the repo anymore
}

build_header

build

package_init

packaging

package_create
