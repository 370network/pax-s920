#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "= build PAXBoard ="
echo "=================="

if [ ! -f Launcher.so ]; then
	echo "PAXBoard build process!"
	make
fi

mkdir -p pkg_output/paxboard/lib
cp Launcher.so pkg_output/paxboard/lib/libosal.so
echo "You can now push the content of the pkg_output folder to your device:"
echo " - paxpush pkg_output/* /data/app/MAINAPP/"

apk.static mkpkg --info arch:armel --files ./pkg_output \
    --info name:PAXBoard \
    --info version:1.1 \
    --info description:"The default homebrew launcher" \
    --info "tags:pax:type=system-only" \
    --output ./PAXBoard-1.1.apk
