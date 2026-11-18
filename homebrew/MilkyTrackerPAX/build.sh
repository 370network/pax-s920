#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build milktrack="
echo "=================="

PKG_NAME="MilkyTrackerPAX"
PKG_DESC="Your favorite excel music player for a payment terminal"
PKG_VER="0.90.87"
PKG_AUTHOR="MilkyTracker contributors, Morc @ 370network"
PKG_DEPENDS="SDL"

echo "Dependency setup!"
cd ../SDL
./build.sh

cd ../MilkyTrackerPAX

git clone https://github.com/370network/MilkyTrackerPAX.git repo

if [ ! -f repo/cmakefilesupdated ]; then
	cp patches/CMakeLists.txt repo/CMakeLists.txt
	echo "done" > repo/cmakefilesupdated
fi

if [ ! -f repo/cmakefilestrackerupdated ]; then
	cp patches/tracker_CMakeLists.txt repo/src/tracker/CMakeLists.txt
	echo "done" > repo/cmakefilestrackerupdated
fi

if [ ! -f repo/build/src/tracker/libmilkytracker.so ]; then
	echo "MilkyTrackerPAX build process!"
	mkdir repo/build
	cd repo/build
	cmake -DSDL_1=1 -DERRATA_3=1 -DCMAKE_CXX_FLAGS="$CXXFLAGS -D__LOWRES__" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_TOOLCHAIN_FILE=../../build_helper.cmake ..
	cd $BUILD_FOLDER
	make -j$(nproc) -C repo/build/
	$STRIP repo/build/src/tracker/libmilkytracker.so
	$STRIP repo/build/src/milkyplay/libmilkyplay.so
	$STRIP repo/build/src/ppui/libppui.so
fi

PKG_NAME_SANITIZED=$(printf '%s' "$PKG_NAME" | LC_ALL=C sed -E 's/[^a-zA-Z0-9._+-]+/-/g; s/^[^a-zA-Z0-9]+//; s/[._+-]+$//')
PKG_DEVICE_DIR="pkg_output/apps/${PKG_NAME_SANITIZED}"

mkdir -p $PKG_DEVICE_DIR/lib
cp icon.png $PKG_DEVICE_DIR
cp "repo/build/src/tracker/libmilkytracker.so" "${PKG_DEVICE_DIR}/${PKG_NAME_SANITIZED}.so"
cp "repo/build/src/milkyplay/libmilkyplay.so" "${PKG_DEVICE_DIR}/lib/libmilkyplay.so"
cp "repo/build/src/ppui/libppui.so" "${PKG_DEVICE_DIR}/lib/libppui.so"

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
