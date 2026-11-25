#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "build libxui_shim "
echo "=================="

PKG_NAME="LibXUI Font Patch"
PKG_DESC="Patcher that forces antialiasing on all fonts rendered by FreeType using the XUI toolkit. It is recommended to install this patch to internal storage."
PKG_VER="0.1"
PKG_AUTHOR="Morc <morc@370.network>"

if [ ! -f libxui_shim.o ]; then
	echo "libxui build process!"
	make -j$(nproc)
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')
PKG_DEVICE_DIR="pkg_output/lib/"

mkdir -p $PKG_DEVICE_DIR

cp "libxui.so" "${PKG_DEVICE_DIR}/libxui.so"

apk.static mkpkg --info arch:armel --files ./pkg_output \
    --info "name:${PKG_NAME_SANITIZED}" \
    --info "version:${PKG_VER}" \
    --info "description:${PKG_DESC}" \
    --info "maintainer:${PKG_AUTHOR}" \
    --output ./${PKG_NAME_SANITIZED}-${PKG_VER}.apk

echo "You can now push the content of the pkg_output folder to your device:"
echo " - paxpush pkg_output/* /data/app/MAINAPP/"
