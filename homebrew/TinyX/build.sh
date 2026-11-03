#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  build tinyx   ="
echo "=================="

echo "Dependency setup!"
cd ../libxfont2
./build.sh

cd ../libxtst
./build.sh

cd ../TinyX

if [ ! -d repo ]; then
	git clone --depth=1 -b proper-libXfont2-port https://github.com/stefan11111/tinyx.git repo
fi

if [ ! -f repo/configure ]; then
	echo "tinyx autogen!"
	cd repo
	NOCONFIGURE=1 ./autogen.sh
	cd ..
fi

for patch_file in "patches"/*.patch; do
	build_patch $patch_file "repo"
done

if [ ! -f repo/kdrive/fbdev/Xfbdev ]; then
	echo "tinyx build process!"
	cd repo
	ax_cv_check_cflags___flto=no ./configure --prefix=$PREFIX --host=$HOST --disable-screensaver --disable-xvesa --enable-xfbdev --disable-xf86bigfont --enable-libXfont2 --disable-dpms CFLAGS="$CFLAGS -fpermissive -std=gnu17" CPPFLAGS="$CPPFLAGS -fpermissive -std=gnu17"
	make -j$(nproc)
	make install
	cd ..
fi
