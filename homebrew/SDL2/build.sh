#!/bin/bash
# build.sh - Morc @ 370network
source $PREFIX/../homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build sdl 2  ="
echo "=================="

git clone https://github.com/libsdl-org/SDL repo

if [ ! -f repo/build/libSDL2.la ]; then
	echo "SDL2 build process!"
	cd repo
	git checkout SDL2
	./configure --disable-pulseaudio --disable-sndio --disable-video --disable-joystick --disable-haptic --disable-hidapi --disable-sensor --disable-video-vulkan --disable-dbus --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	make install
fi
