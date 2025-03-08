#!/bin/bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build sdl 2  ="
echo "=================="

if [ ! -f repo.zip ]; then
	wget https://github.com/libsdl-org/SDL/archive/refs/heads/SDL2.zip -O repo.zip
fi

if [ ! -d repo ]; then
	unzip repo.zip
	mv SDL-SDL2 repo
fi

if [ ! -f repo/build/.libs/libSDL2-2.0.so.0.* ]; then
	echo "SDL2 build process!"
	cd repo
	#git checkout SDL2
	./configure --disable-pulseaudio --disable-sndio --disable-video --disable-joystick --disable-haptic --disable-hidapi --disable-sensor --disable-video-vulkan --disable-dbus --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	$HOST-strip build/.libs/libSDL2-2.0.so.0.*
	make install
fi
