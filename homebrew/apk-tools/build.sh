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
	cp $PREFIX/lib/libapk.so ../libapk.so.3.0.0
	cp $PREFIX/bin/apk ..
fi
