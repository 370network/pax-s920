#!/usr/bin/env bash
# build.sh - Morc @ 370network
source $PAXPATH/homebrew/build_helper.sh

echo "=================="
echo "370network paxdevs"
echo "=   build sdl 2  ="
echo "=================="

git clone https://github.com/370network/SDL2-pax -b 2.0.14-fbdev --depth 1 repo

if [ ! -f repo/build/.libs/libSDL2-2.0.so.0.* ]; then
	echo "SDL2 build process!"
	cd repo
	#git checkout SDL2
	./configure --disable-pulseaudio --disable-sndio --disable-joystick --disable-haptic --disable-hidapi --disable-sensor --disable-video-opengl --disable-video-opengles --disable-video-opengles1 --disable-video-opengles2 --disable-video-vulkan --disable-dbus --enable-video-fbdev --prefix=$PREFIX --host=$HOST
	make -j$(nproc)
	$HOST-strip build/.libs/libSDL2-2.0.so.0.*
	make install
fi
