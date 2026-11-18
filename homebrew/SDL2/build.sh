#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build sdl 2  ="
echo "=================="

PKG_NAME="SDL2"
PKG_DESC="development library designed to provide low level access to audio, keyboard, mouse, joystick and graphics"
PKG_VER="2.0.14"
PKG_AUTHOR="Sam Lantinga & Morc <morc@370.network>"

git clone https://github.com/370network/SDL2-pax -b 2.0.14-fbdev --depth 1 repo

if [ ! -f repo/build/.libs/libSDL2-2.0.so.0.* ]; then
	echo "SDL2 build process!"
	cd repo
	#git checkout SDL2
	./configure --disable-pulseaudio --disable-sndio --disable-joystick --disable-haptic --disable-hidapi --disable-sensor --disable-video-opengl --disable-video-opengles --disable-video-opengles1 --disable-video-opengles2 --disable-video-vulkan --disable-dbus --enable-video-fbdev --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	$HOST-strip build/.libs/libSDL2-2.0.so.0.*
	make install
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')

mkdir -p pkg_output/lib/
cp -P $PREFIX/lib/libSDL2-2.0.so.0 pkg_output/lib/
cp -P $PREFIX/lib/libSDL2.so pkg_output/lib/
cp -P $PREFIX/lib/libSDL2-2.0.so.0.14.0 pkg_output/lib/
apk.static mkpkg --info arch:armel --files ./pkg_output \
    --info "name:${PKG_NAME_SANITIZED}" \
    --info "version:${PKG_VER}" \
    --info "description:${PKG_DESC}" \
    --info "maintainer:${PKG_AUTHOR}" \
    --output ./${PKG_NAME_SANITIZED}_${PKG_VER}.apk

echo "You can now push the content of the pkg_output folder to your device:"
echo " - paxpush pkg_output/* /data/app/MAINAPP/"
