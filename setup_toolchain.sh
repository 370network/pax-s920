#!/bin/bash
# setup_toolchain.sh - Morc @ 370network
# 2nd revision | 2026

echo "=================="
echo "370network paxdevs"
echo "=   toolchain    ="
echo "=================="


echo ""
echo "[*] System info:"
setup_platform=$(uname)
if [ "$setup_platform" == "Linux" ]; then
	setup_platform="linux-gnu"
elif [ "$setup_platform" == "Darwin" ]; then
	setup_platform="apple-darwin"
fi

setup_arch=$(uname -m)
if [ "$setup_arch" == "arm64" ]; then
	setup_arch="aarch64"
fi
echo "$setup_platform on $setup_arch"

echo ""
echo "[*] Cache setup!"
if [ ! -d cache ]; then
    echo "Cache directory create..."
    mkdir cache
else
	echo "Cache already exists, continuing"
fi



echo ""
echo "[*] Toolchain setup!"
if [ ! -f cache/toolchain.tar.xz ]; then
	echo "Toolchain cache download"
	curl -o cache/toolchain.tar.xz -L -O https://github.com/AmanoTeam/obggcc/releases/download/gcc-15/$setup_arch-unknown-$setup_platform.tar.xz
else
	echo "Toolchain cache already exists, continuing"
fi

if [ ! -d toolchain/bin ]; then
	echo "Toolchain unpack..."
	tar -jxf cache/toolchain.tar.xz --strip-components=1 -C $PWD/toolchain \
		obggcc/bin obggcc/build obggcc/lib obggcc/libexec obggcc/usr obggcc/arm-unknown-linux-gnueabi obggcc/arm-unknown-linux-gnueabi2.13
	
	sync
	sleep 1
	
	echo "Toolchain cleanup [1/4]"
	rm -rf toolchain/libexec/gcc/*gnueabihf*
	rm -rf toolchain/libexec/gcc/aarch64*
	rm -rf toolchain/libexec/gcc/x86_64*
	rm -rf toolchain/libexec/gcc/i386*
	
	echo "Toolchain cleanup [2/4]"
	rm -rf toolchain/lib/gcc/*gnueabihf*
	rm -rf toolchain/lib/gcc/aarch64*
	rm -rf toolchain/lib/gcc/x86_64*
	rm -rf toolchain/lib/gcc/i386*
	
	echo "Toolchain cleanup [3/4]"
	rm -rf toolchain/bin/*gnueabihf*
	rm -rf toolchain/bin/aarch64*
	rm -rf toolchain/bin/x86_64*
	rm -rf toolchain/bin/i386*
	
	echo "Toolchain cleanup [4/4]"
	for toolchain_file in toolchain/bin/arm-*; do
		if [[ -f "$toolchain_file" && "$toolchain_file" != *2.13* && "$toolchain_file" != *gnueabi-* ]]; then
			cleanup_target="${dir#toolchain/}"
			#echo "$toolchain_file"
			rm -rf $toolchain_file
		fi
	done
	
	echo "Toolchain linking"
	ln -s $PWD/toolchain/bin/arm-unknown-linux-gnueabi-as toolchain/bin/as
else
	echo "Toolchain already unpacked, continuing"
fi


# gcc ar copy commented out, obggcc has it done correctly
#if [ ! -f toolchain/bin/arm-none-linux-gnueabi-gcc-ar ]; then
#        echo "Toolchain gcc ar copy"
#	cp toolchain/bin/arm-none-linux-gnueabi-ar toolchain/bin/arm-none-linux-gnueabi-gcc-ar
#else
#        echo "Toolchain gcc-ar already copied"
#fi


echo ""
echo "[*] XCB setup!"
if [ ! -d xcb ]; then
    echo "XCB download..."
    git clone https://github.com/370network/prolin-xcb-client xcb --depth=1
else
    echo "XCB already exists"
fi


# substituted as a part of the revised env
#echo "XCB client.py setup!"
#read -p "Enter device address (IP or whole /dev/ path): " addr
#
#if [[ "$addr" =~ ^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+$ ]]; then
#    new_device="device.ConnectDevice(port_path=None, serial=\"$addr:5555\")"
#else
#    new_device="device.ConnectDevice(port_path=None, serial=\"$addr,115200\")"
#fi
#
#if [ "$(uname)" == "Darwin" ]; then
#	sed -i '' "s|device.ConnectDevice(port_path=None, serial=[^)]*)|$new_device|" xcb/client.py
#else
#	sed -i "s|device.ConnectDevice(port_path=None, serial=[^)]*)|$new_device|" xcb/client.py
#fi


echo ""
echo "[*] XCB pre-configuration"
if [ ! -f xcb/bin/activate ]; then
	python3 -m venv xcb
	source xcb/bin/activate
	pip3 install pyserial libusb1 setuptools
	if [ $setup_platform == "apple-darwin" ]; then
		brew_gcc_path=$(brew --prefix gcc)
		export CC=$(ls "/opt/homebrew/opt/gcc/bin/gcc-"* | head -n1)
		export CFLAGS=$(pkg-config --cflags openssl)
		export LDFLAGS=$(pkg-config --libs openssl)
		export SWIG_FEATURES="-cpperraswarn -includeall $(pkg-config --cflags openssl)"
		pip3 install --pre --no-binary :all: M2Crypto --no-cache
	else
		pip3 install M2Crypto==0.40.0
	fi
else
	echo "XCB pre-configuration already done, continuing"
fi


echo ""
echo "[*] Finish"
echo "Initial setup done!"
echo "In case of errors, please use your eyes and read. Continue by opening https://github.com/370network/pax-s920/issues and report problems."
