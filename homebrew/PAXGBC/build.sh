#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build paxgbc  ="
echo "=================="

PKG_NAME="PAXGBC"
PKG_DESC="Game Boy Color Emulator"
PKG_VER="1.0.0"
PKG_AUTHOR="rhgndf"

git clone https://github.com/rhgndf/paxgbc repo

if [ ! -f repo/cmakelistsupdated ]; then
	cp CMakeLists.txt repo/CMakeLists.txt
	echo "done" > repo/cmakelistsupdated
fi

if [ ! -f repo/mainupdated ]; then
	cp main.cpp repo/main.cpp
	echo "done" > repo/mainupdated
fi

if [ ! -f repo/libpaxgbc.so ]; then
        echo "paxgbc build process!"
        cd repo
        cmake -DCMAKE_TOOLCHAIN_FILE=../../build_helper.cmake .
        make -j$(nproc)
        $STRIP libpaxgbc.so
        cd ..
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')
PKG_DEVICE_DIR="pkg_output/apps/${PKG_NAME_SANITIZED}"

mkdir -p $PKG_DEVICE_DIR
cp icon.png $PKG_DEVICE_DIR
cp bbmode1_8x8.png $PKG_DEVICE_DIR
cp -r roms $PKG_DEVICE_DIR/

cp "repo/libpaxgbc.so" "${PKG_DEVICE_DIR}/${PKG_NAME_SANITIZED}.so"

{
    printf 'name: %s\n' "$PKG_NAME"
    printf 'description: %s\n' "$PKG_DESC"
    printf 'version: %s\n' "$PKG_VER"
    printf 'author: %s\n' "$PKG_AUTHOR"
    [ -z "$PKG_LICENSE" ] || printf 'license: %s\n' "$LICENSE"
    [ -z "$PKG_URL" ] || printf 'url: %s\n' "$URL"
} > "${PKG_DEVICE_DIR}/info.txt"


apk.static mkpkg --info arch:armel --files ./pkg_output \
    --info "name:${PKG_NAME_SANITIZED}" \
    --info "version:${PKG_VER}" \
    --info "description:${PKG_DESC}" \
    --info "maintainer:${PKG_AUTHOR}" \
    --output ./${PKG_NAME_SANITIZED}_${PKG_VER}.apk

echo "You can now push the content of the pkg_output folder to your device:"
echo " - paxpush pkg_output/* /data/app/MAINAPP/"
