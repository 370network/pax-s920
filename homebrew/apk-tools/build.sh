#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="libapk"
PKG_DESC="Alpine Package Keeper - library"
PKG_VER="3.0.9-r0"
PKG_AUTHOR="Alpine Linux Project & Morc <morc@370.network>"

_build() {
	git clone https://github.com/alpinelinux/apk-tools.git repo

	build_apply_all_patches "repo"

	if [ ! -f repo/build/src/libapk.so.3.0.0 ]; then
		cd repo
		meson setup build --cross-file $PAXPATH/homebrew/build_helper.meson --prefix=$PREFIX --buildtype=plain	
		meson configure build -Dminimal=true -Ddefault_library=shared -Dcrypto_backend=openssl -Durl_backend=libfetch -Dhelp=enabled -Ddocs=disabled -Dlua=disabled -Dpython=disabled -Dtests=disabled -Dzstd=disabled -Dlua_bin=$(which lua)
		ninja -C build install
		cd ..
	fi
}

build_header

build

#libapk
package_init

_packaging() {
	cp $PREFIX/lib/libapk.so $PKG_LIB_DIR/libapk.so.3.0.0
}

packaging

package_create



#apk cli tool
PKG_NAME="apk-tools"
PKG_DESC="Alpine Package Keeper - cli utility"
PKG_DEPENDS="libapk"
package_init

_packaging() {
	cp $PREFIX/bin/apk $PKG_BIN_DIR/
}

packaging

package_create
