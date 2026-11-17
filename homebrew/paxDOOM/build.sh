#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build paxDOOM  ="
echo "=================="

PKG_NAME="paxDOOM"
PKG_DESC="A DOOM port based on fbdoom"
PKG_VER="1.0.0"
PKG_AUTHOR="ID Software, maximevince, Morc @ 370network"

git clone https://github.com/370network/paxDOOM.git repo

export CROSS_COMPILE=$TOOLCHAIN/bin/$HOST-

if [ ! -f repo/fbdoom/fbdoom ]; then
	echo "paxDOOM build process!"
	make -j$(nproc) -C repo/fbdoom/
	$STRIP repo/fbdoom/fbdoom
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')
PKG_DEVICE_DIR="pkg_output/apps/${PKG_NAME_SANITIZED}"

mkdir -p $PKG_DEVICE_DIR
cp icon.png $PKG_DEVICE_DIR

build_check "fbdoom/fbdoom" "${PKG_DEVICE_DIR}/${PKG_NAME_SANITIZED}.so"

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
