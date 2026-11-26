#!/bin/sh
# build.sh - Morc @ 370network
. $PAXPATH/homebrew/build_helper.sh

PKG_NAME="SDL2"
PKG_DESC="development library designed to provide low level access to audio, keyboard, mouse, joystick and graphics"
PKG_VER="2.0.14"
PKG_AUTHOR="Sam Lantinga & Morc <morc@370.network>"

_build() {
	if [ ! -d repo ]; then
		git clone https://github.com/370network/SDL2-pax -b $PKG_VER-fbdev --depth 1 repo
	fi

	if [ ! -f repo/build/.libs/libSDL2-2.0.so.0.* ]; then
		cd repo
		./configure --disable-pulseaudio --disable-sndio --disable-joystick --disable-haptic --disable-hidapi --disable-sensor --disable-video-opengl --disable-video-opengles --disable-video-opengles1 --disable-video-opengles2 --disable-video-vulkan --disable-dbus --enable-video-fbdev --prefix=$PREFIX --host=$HOST
		make -j$(nproc) install
	fi
}

_packaging() {
	cp -P $PREFIX/lib/libSDL2-2.0.so.0 $PKG_LIB_DIR
	cp -P $PREFIX/lib/libSDL2.so $PKG_LIB_DIR
	cp -P $PREFIX/lib/libSDL2-2.0.so.0.14.0 $PKG_LIB_DIR
}

build_header

build

package_init

packaging

package_create