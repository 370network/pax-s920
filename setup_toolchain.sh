#!/usr/bin/env bash
# setup_toolchain.sh - Morc @ 370network
# 2nd revision | 2026

echo "=================="
echo "370network paxdevs"
echo "=   toolchain    ="
echo "=================="


echo ""
echo "[*] System info:"
setup_platform=$(uname)
setup_libc=$(ldd --version)
if [ "$setup_platform" == "Linux" ]; then
	if [[ "$setup_libc" == *"GLIBC"* ]]; then
		setup_platform="linux-gnu"
	else
		setup_platform="linux-musl"
	fi
elif [ "$setup_platform" == "Darwin" ]; then
	setup_platform="apple-darwin"
fi

setup_distro="generic"
if [ -f /etc/os-release ]; then
	. /etc/os-release
	setup_distro=$ID
elif [ -f /etc/lsb-release]; then
	. /etc/lsb-release
	setup_distro=$ID
fi

setup_arch=$(uname -m)
if [ "$setup_arch" == "arm64" ]; then
	setup_arch="aarch64"
fi
echo "$setup_platform $setup_distro on $setup_arch"

check_package_dpkg(){
	PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $1|grep "install ok installed")
	if [ "" = "$PKG_OK" ]; then
		echo "[-] $1 missing. installing $1..."
		sudo apt-get -qq --yes install $1
	else
		echo "$1 installed"
	fi
}

echo ""
echo "[*] Distro/system specific package checkup!"
if [ "$setup_distro" == "generic" ]; then
	echo "Running an unidentified distro, skipping checks..."
elif [[ "${setup_distro,,}" = *"debian"* || "${setup_distro,,}" = *"ubuntu"* ]]; then
	check_package_dpkg "python3-venv"
	check_package_dpkg "swig"
	check_package_dpkg "pkg-config"
	check_package_dpkg "build-essential"
	check_package_dpkg "git"
	check_package_dpkg "qemu-user-static"
	check_package_dpkg "cmake"
	check_package_dpkg "libssl-dev"
	check_package_dpkg "python3-dev"
	check_package_dpkg "m4"
fi



echo ""
echo "[*] Cache setup!"
if [ ! -d cache ]; then
    echo "Cache directory create..."
    mkdir cache
else
	echo "Cache already exists, continuing"
fi

gcc_ver=15

echo ""
echo "[*] GCC $gcc_ver Toolchain setup!"
if [ ! -f cache/toolchain.tar.xz ]; then
	echo "GCC Toolchain cache download"
	curl -o cache/toolchain.tar.xz -L -O https://github.com/AmanoTeam/obggcc/releases/download/gcc-$gcc_ver/$setup_arch-unknown-$setup_platform.tar.xz
else
	echo "GCC Toolchain cache already exists, continuing"
fi

if [ ! -d toolchain/bin ]; then
	echo "GCC $gcc_ver Toolchain unpack..."
	tar -xf cache/toolchain.tar.xz --strip-components=1 -C $PWD/toolchain \
		obggcc/bin obggcc/build obggcc/lib obggcc/libexec obggcc/usr obggcc/arm-unknown-linux-gnueabi obggcc/arm-unknown-linux-gnueabi2.13

	sync
	sleep 1

	echo "GCC Toolchain cleanup [1/4]"
	rm -rf toolchain/libexec/gcc/*gnueabihf*
	rm -rf toolchain/libexec/gcc/aarch64*
	rm -rf toolchain/libexec/gcc/x86_64*
	rm -rf toolchain/libexec/gcc/i386*

	echo "GCC Toolchain cleanup [2/4]"
	rm -rf toolchain/lib/gcc/*gnueabihf*
	rm -rf toolchain/lib/gcc/aarch64*
	rm -rf toolchain/lib/gcc/x86_64*
	rm -rf toolchain/lib/gcc/i386*

	echo "GCC Toolchain cleanup [3/4]"
	rm -rf toolchain/bin/*gnueabihf*
	rm -rf toolchain/bin/aarch64*
	rm -rf toolchain/bin/x86_64*
	rm -rf toolchain/bin/i386*

	echo "GCC Toolchain cleanup [4/4]"
	for toolchain_file in toolchain/bin/arm-*; do
		if [[ -f "$toolchain_file" && "$toolchain_file" != *2.13* && "$toolchain_file" != *gnueabi-* ]]; then
			cleanup_target="${dir#toolchain/}"
			#echo "$toolchain_file"
			rm -rf $toolchain_file
		fi
	done

	echo "GCC Toolchain linking"
	ln -s $PWD/toolchain/bin/arm-unknown-linux-gnueabi-as toolchain/bin/as
else
	echo "GCC Toolchain already unpacked, continuing"
fi

echo ""
echo "[*] GCC $gcc_ver Toolchain libs!"
if [ ! -f cache/lib.tar.gz ]; then
	echo "GCC Toolchain lib cache download"
	curl -o cache/lib.tar.gz -L -O https://forum.370.network/download/file.php?id=927
else
	echo "GCC Toolchain lib cache already exists, continuing"
fi

if [ ! -f toolchain/arm-unknown-linux-gnueabi/lib/libosal.so ]; then
	echo "GCC Toolchain lib unpack..."
	tar -xf cache/lib.tar.gz -C $PWD/toolchain/arm-unknown-linux-gnueabi/lib
else
	echo "GCC Toolchain lib already unpacked, continuing"
fi


echo ""
echo "[*] XCB setup!"
if [ ! -d xcb ]; then
    echo "XCB download..."
    git clone https://github.com/370network/prolin-xcb-client xcb --depth=1
else
    echo "XCB already exists"
fi

setup_xcb(){
	python3 -m venv xcb
	source xcb/bin/activate
	pip3 install pyserial libusb1 setuptools
	if [ $setup_platform == "apple-darwin" ]; then
		echo "M2Crypto Darwin Brew build"
		brew_gcc_path=$(brew --prefix gcc)
		export CC=$(ls "/opt/homebrew/opt/gcc/bin/gcc-"* | head -n1)
		export CFLAGS=$(pkg-config --cflags openssl)
		export LDFLAGS=$(pkg-config --libs openssl)
		export SWIG_FEATURES="-cpperraswarn -includeall $(pkg-config --cflags openssl)"
		pip3 install --pre --no-binary :all: M2Crypto --no-cache
	elif [[ "${setup_distro,,}" = *"debian"* ]]; then
		echo "M2Crypto Linux build"
		export CFLAGS=$(pkg-config --cflags openssl)
		export LDFLAGS=$(pkg-config --libs openssl)
		export SWIG_FEATURES="-cpperraswarn -includeall $(pkg-config --cflags openssl)"
		pip3 install --pre --no-binary :all: M2Crypto --no-cache
	else
		echo "M2Crypto fallback"
		pip3 install M2Crypto==0.40.0
	fi
}

echo ""
echo "[*] XCB pre-configuration!"
if [ ! -f xcb/bin/activate ]; then
	setup_xcb
else
	echo "XCB resetting pre-configuration"
	rm -rf xcb/pyenv.cfg xcb/lib xcb/bin xcb/include
	setup_xcb
fi

echo ""
echo "[*] Finish!"
echo "Initial setup done!"
echo "In case of errors, please use your eyes and read. Continue by opening https://github.com/370network/pax-s920/issues and report problems."
