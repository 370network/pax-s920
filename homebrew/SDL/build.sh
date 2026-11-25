#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build sdl 1  ="
echo "=================="

PKG_NAME="SDL"
PKG_DESC="development library designed to provide low level access to audio, keyboard, mouse, joystick and graphics"
PKG_VER="1.2.16"
PKG_AUTHOR="Sam Lantinga & Morc <morc@370.network>"

git clone https://github.com/370network/SDL-1.2-pax.git repo

if [ ! -f repo/build/.libs/libSDL-1.2.so.0.11.5 ]; then
	echo "SDL1.2 build process!"
	cd repo
	./configure --disable-pulseaudio --prefix=$PREFIX --host=$HOST_GLIBC
	make -j$(nproc)
	$STRIP build/.libs/libSDL-1.2.so.0.11.5
	make install
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')

mkdir -p pkg_output/lib/
cp -P $PREFIX/lib/libSDL-1.2.so.0.11.5 pkg_output/lib/
cp -P $PREFIX/lib/libSDL-1.2.so.0 pkg_output/lib/
apk.static mkpkg --info arch:armel --files ./pkg_output \
    --info "name:${PKG_NAME_SANITIZED}" \
    --info "version:${PKG_VER}" \
    --info "description:${PKG_DESC}" \
    --info "maintainer:${PKG_AUTHOR}" \
    --output ./${PKG_NAME_SANITIZED}-${PKG_VER}.apk

echo "You can now push the content of the pkg_output folder to your device:"
echo " - paxpush pkg_output/* /data/app/MAINAPP/"
