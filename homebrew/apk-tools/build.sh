#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build apk-tools="
echo "=================="

git clone https://github.com/alpinelinux/apk-tools.git repo

for patch_file in "patches"/*.patch; do
	build_patch $patch_file "repo"
done

if [ ! -f repo/libz.so.1.2.7 ]; then
	echo "apk-tools build process!"
	cd repo
	meson setup build --cross-file $PAXPATH/homebrew/build_helper.meson --prefix=$PREFIX --buildtype=plain	
	meson configure build -Dminimal=true -Ddefault_library=shared -Dcrypto_backend=openssl -Durl_backend=libfetch -Dhelp=enabled -Ddocs=disabled -Dlua=disabled -Dpython=disabled -Dtests=disabled -Dzstd=disabled -Dlua_bin=$(which lua)
	ninja -C build install
fi

cd ..
mkdir -p pkg_output/lib/
cp $PREFIX/lib/libapk.so pkg_output/lib/libapk.so.3.0.0
apk.static mkpkg --info arch:armel --files ./pkg_output \
    --info name:libapk \
    --info version:3.0.9-r0 \
    --info description:"Alpine Package Keeper - package manager for alpine" \
    --info "tags:pax:type=system-only" \
    --output ./libapk_3.0.9-r0.apk

mv pkg_output/lib/ .

mkdir -p pkg_output/bin/
cp $PREFIX/bin/apk pkg_output/bin/

apk.static mkpkg --info arch:armel --files ./pkg_output \
    --info name:apk-tools \
    --info version:3.0.9 \
    --info description:"Alpine Package Keeper - package manager for alpine" \
    --info "tags:pax:type=system-only" \
    --info depends:libapk \
    --output ./apk-tools_3.0.9-r0.apk

mv lib/ pkg_output/

echo "You can now push the content of the pkg_output folder to your device:"
echo " - paxpush pkg_output/* /data/app/MAINAPP/"
