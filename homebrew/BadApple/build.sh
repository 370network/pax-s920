#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build BadApple ="
echo "=================="

PKG_NAME="Bad Apple"
PKG_DESC="Bad Apple"
PKG_VER="1.0.0"
PKG_AUTHOR="rhgndf"


git clone https://github.com/paxdevs/paxdemo repo

if [ ! -f repo/cmakelistsupdated ]; then
	cp patches/CMakeLists.txt repo/CMakeLists.txt
	echo "done" > repo/cmakelistsupdated
fi

if [ ! -f repo/mainupdated ]; then
	cp patches/main.cpp repo/main.cpp
	echo "done" > repo/mainupdated
fi

if [ ! -f repo/audioupdated ]; then
	cp patches/audio.cpp repo/audio.cpp
	echo "done" > repo/audioupdated
fi

if [ ! -f repo/libpaxdemo.so ]; then
        echo "BadApple build processs!"
        cd repo
        cmake -DCMAKE_TOOLCHAIN_FILE=../../build_helper.cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 .
        make -j$(nproc)
        $STRIP libpaxdemo.so
        cd ..
fi

mkdir -p pkg_output/apps/BadApple/
if [ ! -f pkg_output/apps/BadApple/video.frames ]; then
	cp repo/video.frames pkg_output/apps/BadApple/
fi
if [ ! -f pkg_output/apps/BadApple/BadApple/audio.opus ]; then
	cp repo/audio.opus pkg_output/apps/BadApple/
fi
cp icon.png pkg_output/apps/BadApple/

build_check "libpaxdemo.so" "pkg_output/apps/BadApple/BadApple.so"

{
    printf 'name: %s\n' "$PKG_NAME"
    printf 'description: %s\n' "$PKG_DESC"
    printf 'version: %s\n' "$PKG_VER"
    printf 'author: %s\n' "$PKG_AUTHOR"
    [ -z "$PKG_LICENSE" ] || printf 'license: %s\n' "$LICENSE"
    [ -z "$PKG_URL" ] || printf 'url: %s\n' "$URL"
} > "pkg_output/apps/BadApple/info.txt"

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')

apk.static mkpkg --info arch:armel --files ./pkg_output \
    --info "name:${PKG_NAME_SANITIZED}" \
    --info "version:${PKG_VER}" \
    --info "description:${PKG_DESC}" \
    --info "maintainer:${PKG_AUTHOR}" \
    --output ./${PKG_NAME_SANITIZED}_${PKG_VER}.apk

echo "You can now push the content of the pkg_output folder to your device:"
echo " - paxpush pkg_output/* /data/app/MAINAPP/"
