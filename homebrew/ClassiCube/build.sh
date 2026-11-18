#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=build ClassiCube="
echo "=================="

PKG_NAME="ClassiCube"
PKG_DESC="Custom Minecraft Classic compatible client"
PKG_VER="1.3.9"
PKG_AUTHOR="UnknownShadow200 and contributors & Morc @ 370network"
PKG_DEPENDS="SDL2"

git clone https://github.com/ClassiCube/ClassiCube --depth 1 repo

for patch_file in "patches"/*.patch; do
	build_patch $patch_file "repo"
done

if [ ! -f repo/ClassiCube ]; then
	echo "ClassiCube build process!"
	make -j$(nproc) -C repo/ linux BUILD_SDL2=1
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')
PKG_DEVICE_DIR="pkg_output/apps/${PKG_NAME_SANITIZED}"

mkdir -p $PKG_DEVICE_DIR
cp icon.png $PKG_DEVICE_DIR
cp ClassiCube_Loader $PKG_DEVICE_DIR/$PKG_NAME_SANITIZED
cp "repo/ClassiCube" "${PKG_DEVICE_DIR}/ClassiCube_bin"

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
    --info "depends:${PKG_DEPENDS}" \
    --output ./${PKG_NAME_SANITIZED}_${PKG_VER}.apk

echo "You can now push the content of the pkg_output folder to your device:"
echo " - paxpush pkg_output/* /data/app/MAINAPP/"
