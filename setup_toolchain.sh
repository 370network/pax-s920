#!/bin/bash
# setup_toolchain.sh - Morc @ 370network

echo "=================="
echo "370network paxdevs"
echo "=   toolchain    ="
echo "=================="



echo ""
echo ""
echo "Cache setup!"
if [ ! -d cache ]; then
        echo "Cache directory create"
        mkdir cache
fi



echo ""
echo ""
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



echo ""
echo ""
echo "XCB setup!"
if [ -d xcb ]; then
        echo "XCB already exists"
else
        echo "XCB download"
        git clone https://git.lsd.cat/g/prolin-xcb-client.git xcb --depth=1
fi

echo "XCB client.py setup!"
read -p "Enter device address (IP or whole /dev/ path): " addr

if [[ "$addr" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
    new_device="device.ConnectDevice(port_path=None, serial=\"$addr:5555\")"
else
    new_device="device.ConnectDevice(port_path=None, serial=\"$addr,115200\")"
fi

if [ "$(uname)" == "Darwin" ]; then
	sed -i '' "s|device.ConnectDevice(port_path=None, serial=[^)]*)|$new_device|" xcb/client.py
else
	sed -i "s|device.ConnectDevice(port_path=None, serial=[^)]*)|$new_device|" xcb/client.py
fi

echo "XCB env"
python3 -m venv xcb
source xcb/bin/activate
pip3 install pyserial libusb1 setuptools
if [ "$(uname)" == "Darwin" ]; then
	export CFLAGS=$(pkg-config --cflags openssl)
	export LDFLAGS=$(pkg-config --libs openssl)
	export SWIG_FEATURES="-cpperraswarn -includeall $(pkg-config --cflags openssl)"
	pip3 install --pre --no-binary :all: M2Crypto --no-cache
else
	pip3 install M2Crypto==0.40.0
fi

echo ""
echo ""
echo "Initial setup done!"
echo "In case of errors, please use your eyes and read. Continue by opening https://github.com/370network/pax-s920/issues and report problems."
