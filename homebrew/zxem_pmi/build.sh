#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build hax-pax  ="
echo "=================="

git clone https://github.com/Sprotex/hax-pax/ repo

if [ ! -f repo/makefileupdated ]; then
	cp patches/Makefile repo/Makefile
	echo "done" > repo/makefileupdated
fi

if [ ! -f repo/zxemupdated ]; then
	cp patches/zxem.c repo/src/zxem.c
	echo "done" > repo/zxemupdated
fi

PKG_NAME="ZX Spectrum Emulator"
PKG_DESC="ZX Spectrum Emulator"
PKG_VER="1.0.0"
PKG_AUTHOR="Sprotex & ikonko"

if [ ! -f repo/zxem ]; then
	echo "zxem build process!"
	make -j$(nproc) -C repo zxem
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')
PKG_DEVICE_DIR="pkg_output/apps/${PKG_NAME_SANITIZED}"

mkdir -p $PKG_DEVICE_DIR/
build_check "zxem" "${PKG_DEVICE_DIR}/${PKG_NAME_SANITIZED}.so"
cp zxem_icon.png "${PKG_DEVICE_DIR}/icon.png"
cp zx48.rom "${PKG_DEVICE_DIR}/"
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

rm -rf pkg_output

PKG_NAME="PMI-80"
PKG_DESC="TESLA PMI-80 Emulator"
PKG_VER="1.0.0"
PKG_AUTHOR="Sprotex"

if [ ! -f repo/pmi ]; then
        echo "pmi build process!"
        make -j$(nproc) -C repo pmi
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')
PKG_DEVICE_DIR="pkg_output/apps/${PKG_NAME_SANITIZED}"

mkdir -p $PKG_DEVICE_DIR/
build_check "pmi" "${PKG_DEVICE_DIR}/${PKG_NAME_SANITIZED}.so"
cp pmi_icon.png "${PKG_DEVICE_DIR}/icon.png"
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
