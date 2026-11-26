#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="Basilisk II"
PKG_DESC="Macintosh 68k Emulator"
PKG_VER="1.0"
PKG_AUTHOR="Christian Bauer et al, Adam and Morc @ 370network"
PKG_ISAPP=1
PKG_DEPENDS="SDL gmp mpfr"

build_dependency "gmp"
build_dependency "mpfr"
build_dependency "SDL"

_build() {
	if [[ $env_platform != "linux-gnu" ]] then
		_pkg_log_error "You are currently building on $env_platform, this environment is UNSUPPORTED!"
		_pkg_log_error "BasiliskII compilation process currently depends on a glibc Linux environment for obscure reasons!"
		_pkg_log_error "If you want to compile BasiliskII, you have to do so elsewhere. Sorry :("
		exit -370
	fi
	
	if [[ $(getconf PAGESIZE) != 4096 ]]; then
	        _pkg_log_error "You are currently building on a platform that has a $(getconf PAGESIZE) byte kernel page size!"
	        _pkg_log_error "BasiliskII compilation process currently depends on a platform with 4096 bytes!"
	        _pkg_log_error "If you want to compile BasiliskII, you have to do so elsewhere. Sorry :("
	        exit -370
	fi
	
	if [[ $(command -v qemu-arm) != "" ]] || [[ $(command -v qemu-system-arm) != "" ]] then
	        _pkg_log_error "You are missing the qemu-user package!"
	        _pkg_log_error "BasiliskII compilation process currently depends on qemu-user (qemu-arm/qemu-system-arm) to work.!"
	        _pkg_log_error "If you want to compile BasiliskII, you have to do so elsewhere. Sorry :("
	        exit1 -370
	fi
	
	if [ ! -d repo-macemu ]; then
		git clone https://github.com/kanjitalk755/macemu --depth 1 repo-macemu
	fi
	
	build_apply_all_patches "repo-macemu"
	
	if [ ! -d repo ]; then
		cp -r repo-macemu/BasiliskII repo
	fi
	
	if [ ! -f repo/src/Unix/Makefile ]; then
		cd repo/src/Unix
		unset CC
		unset CXX
		unset LD
		unset GXX
		unset AS
		unset STRIP
		unset LD_LIBRARY_PATH
		unset CPPFLAGS
		unset LDFLAGS
		unset LIBS
		NO_CONFIGURE=1 ./autogen.sh
		./configure --host=$HOST_GLIBC --prefix=$PREFIX --with-sdl1 BII_CROSS_HAVE_EXTENDED_SIGNALS=yes CPPFLAGS="-I$PREFIX/include" LDFLAGS="-L$PREFIX/lib" sdl_LIBS="-lSDL" LIBS="-lSDL" sdl_CFLAGS="-I$PREFIX/include/SDL"
	fi
	
	if [ ! -f repo/src/Unix/BasiliskII ]; then
		make -j$(nproc)
	fi
}

_packaging() {
	cp "repo/src/Unix/BasiliskII" "$PKG_APP_DIR/BasiliskII_bin"
	cp "icon.png" "$PKG_APP_DIR/"
	cp "keycodes_iso" "$PKG_APP_DIR/"
	cp "BasiliskII_Loader" "$PKG_APP_DIR/$_pkg_name"
}

build_header

build

package_init

packaging

package_create
