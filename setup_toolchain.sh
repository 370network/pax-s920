#!/bin/bash
# setup_toolchain.sh - Morc @ 370network

echo "=================="
echo "370network paxdevs"
echo "=   toolchain    ="
echo "=================="



echo "Cache setup!"
if [ ! -d cache ]; then
        echo "Cache directory create"
        mkdir cache
fi



echo "Toolchain setup!"
if [ -f cache/toolchain.tar.bz2 ]; then
	echo "Toolchain already exists"
else
	echo "Toolchain download"
	wget https://forum.370.network/download/file.php?id=287 -O cache/toolchain.tar.bz2
fi

if [ ! -d toolchain/bin ]; then
	echo "Toolchain unpack"
	tar -jxf cache/toolchain.tar.bz2 --strip-components=1 -C $PWD/toolchain
else
	echo "Toolchain already unpacked"
fi



echo "XCB setup!"
if [ -d xcb ]; then
        echo "XCB already exists"
else
        echo "XCB download"
        git clone https://git.lsd.cat/g/prolin-xcb-client.git xcb --depth=1
fi

echo "XCB client.py setup!"
read -p "Enter device address (IP or /dev/tty path): " addr

if [[ "$addr" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    new_device="device.ConnectDevice(port_path=None, serial=\"$addr:5555\")"
else
    new_device="device.ConnectDevice(port_path=None, serial=\"$addr,115200\")"
fi

sed -i "s|device.ConnectDevice(port_path=None, serial=[^)]*)|$new_device|" xcb/client.py

echo "XCB env"
python3 -m venv xcb
source xcb/bin/activate
pip3 install M2Crypto pyserial libusb1
