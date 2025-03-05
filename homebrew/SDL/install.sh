#!/bin/bash
# install.sh - Morc @ 370network
source $PREFIX/../homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=  install sdl1  ="
echo "=================="

if [ ! -d $PREFIX/include/SDL ]; then
	mkdir $PREFIX/include/SDL
fi

wget https://github.com/370network/SDL-1.2-pax/releases/download/1.2/SDL-1.2-pax_USE_FOR_BUILDING_OTHER_PORTS.zip -O install.zip

unzip install.zip

mv lib/* $PREFIX/lib
mv include/* $PREFIX/include/SDL
rm -rf __MACOSX lib include

echo "SDL1 should be installed now!"
