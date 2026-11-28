#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="TinyX"
PKG_DESC="The smallest fully featured X server binary"
PKG_VER="1.3"
PKG_AUTHOR="tinycorelinux, stefan11111 & Morc <morc@370.network>"
PKG_ISAPP=1
PKG_DEPENDS="libXfont2 libXtst xfonts-base"

build_dependency "libxfont2"
build_dependency "libxtst"

_build() {
	if [ ! -d repo ]; then
		git clone --depth=1 -b proper-libXfont2-port https://github.com/stefan11111/tinyx.git repo
	fi

	if [ ! -f repo/configure ]; then
		cd repo
		NOCONFIGURE=1 ./autogen.sh
		cd ..
	fi

	build_apply_all_patches "repo"

	if [ ! -f repo/kdrive/fbdev/Xfbdev ]; then
		cd repo
		ax_cv_check_cflags___flto=no ./configure --prefix=$PREFIX --host=$HOST --disable-screensaver --disable-xvesa --enable-xfbdev --disable-xf86bigfont --enable-libXfont2 --disable-dpms CFLAGS="$CFLAGS -fpermissive -std=gnu17" CPPFLAGS="$CPPFLAGS -fpermissive -std=gnu17"
		make -j$(nproc) install
		cd ..
	fi
}

_packaging() {
	cp icon.png $PKG_APP_DIR
	cp "repo/kdrive/fbdev/Xfbdev" "$PKG_APP_DIR/Xfbdev"
	cp "TinyX_loader" "$PKG_APP_DIR/TinyX_loader"
	cp "TinyX_script" "$PKG_APP_DIR/$_pkg_name"
}

build_header

build

package_init

packaging

package_create
