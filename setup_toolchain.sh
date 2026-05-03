#!/usr/bin/env bash
# setup_toolchain.sh - Morc @ 370network
# 2nd revision | 2026

echo "=================="
echo "370network paxdevs"
echo "=   toolchain    ="
echo "=================="


echo ""
echo "[*] System info:"
eval "$(bash platform.sh)"
echo "$env_platform $env_distro on $env_arch"

check_package_dpkg(){
	PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $1|grep "install ok installed")
	if [ "" = "$PKG_OK" ]; then
		echo "[-] $1 missing. installing $1..."
		sudo apt-get -qq --yes install $1
	else
		echo "$1 installed"
	fi
}

check_package_apk(){
	if ! printf '%s\n' "$package_generate_list" | grep -Fxq "$1"; then
		echo "[-] $1 missing. installing $1..."
		sudo apk add -q $1 --no-interactive
	else
		echo "$1 installed"
	fi
}

check_package_rpmdnf(){
    if ! printf '%s\n' "$package_generate_list" | grep -Fxq "$1"; then
		echo "[-] $1 missing. installing $1..."
		sudo dnf install $1 -q -y
	else
		echo "$1 installed"
	fi
}

check_package_brew(){
	grep_arg="${2:- -Fqx}"
    if ! printf '%s\n' "$package_generate_list" | grep $grep_arg "$1"; then
		echo "[-] $1 missing. installing $1..."
		brew install $1 -q -f
	else
		echo "$1 installed"
	fi
}

check_package_pacman(){
    if ! printf '%s\n' "$package_generate_list" | grep -Fxq "$1"; then
		echo "[-] $1 missing. installing $1..."
		pacman -Sy $1 --quiet --noconfirm
	else
		echo "$1 installed"
	fi
}


echo ""
echo "[*] Distro/system specific package checkup!"
if [[ "$env_distro" = *"debian"* || "$env_distro" = *"ubuntu"* ]]; then
	check_package_dpkg "python3-venv"
	check_package_dpkg "pkg-config"
	check_package_dpkg "build-essential"
	check_package_dpkg "git"
	check_package_dpkg "qemu-user-static"
	check_package_dpkg "cmake"
	check_package_dpkg "m4"
	check_package_dpkg "autoconf"
	check_package_dpkg "libarchive-tools"
elif [[ "$env_distro" = *"postmarketos"* || "$env_distro" = *"alpine"* ]]; then
	echo "Getting apk package list..."
	package_generate_list=$(apk info)
	check_package_apk "curl"
	check_package_apk "git"
	check_package_apk "m4"
	check_package_apk "autoconf"
	check_package_apk "automake"
	check_package_apk "cmake"
	check_package_apk "build-base"
	check_package_apk "pkgconf"
	check_package_apk "py3-pip"
	check_package_apk "libarchive-tools"
elif [[ "$env_distro" = *"fedora"* ]]; then
	package_generate_list=$(rpm -qa --qf '%{NAME}\n')
	check_package_rpmdnf "python3"
	check_package_rpmdnf "python3-pip"
	check_package_rpmdnf "gcc"
	check_package_rpmdnf "cmake"
	check_package_rpmdnf "m4"
	check_package_rpmdnf "automake"
	check_package_rpmdnf "make"
	check_package_rpmdnf "gcc-c++"
	check_package_rpmdnf "openssl"
	check_package_rpmdnf "bsdtar"
elif [[ "$env_platform" = *"darwin"* ]]; then
	package_generate_list=$(brew list -1)
	check_package_brew "libarchive"
	check_package_brew "bash"
	check_package_brew "m4"
	check_package_brew "autoconf"
	check_package_brew "automake"
	check_package_brew "cmake"
	check_package_brew "pkgconf"
	check_package_brew "gcc"
	check_package_brew "python@3" "-q"
elif [ "$env_distro" == "msys2" ]; then
	package_generate_list=$(pacman -Q | awk '{print $1}')
	check_package_pacman "git"
	check_package_pacman "bsdtar"
	check_package_pacman "python"
	check_package_pacman "gcc"
	check_package_pacman "make"
	check_package_pacman "cmake"
	check_package_pacman "automake-wrapper"
	check_package_pacman "autoconf-wrapper"
	check_package_pacman "pkgconf"
	check_package_pacman "m4"
	check_package_pacman "openssl"
elif [ "$env_distro" == "arch" ]; then
        package_generate_list=$(pacman -Q | awk '{print $1}')
        check_package_pacman "git"
        check_package_pacman "libarchive"
        check_package_pacman "python"
        check_package_pacman "gcc"
        check_package_pacman "make"
        check_package_pacman "cmake"
        check_package_pacman "automake"
        check_package_pacman "autoconf"
        check_package_pacman "pkgconf"
        check_package_pacman "openssl"
elif [[ "$env_distro" = *"nixos"* ]]; then
	echo "Package dependencies have been already handled by nix-shell, continuing..."
elif [ "$env_distro" == "generic" ]; then
	echo "Running an unidentified distro, skipping checks..."
fi


echo ""
echo "[*] Cache setup!"
if [ ! -d cache ]; then
    echo "Cache directory create..."
    mkdir cache
else
	echo "Cache already exists, continuing"
fi


patch_toolchain_nixos(){
	for toolchain_file in $1; do
		echo "patching file: $toolchain_file to use $(cat $NIX_CC/nix-support/dynamic-linker) as interpreter"
		patchelf "$toolchain_file" > /dev/null 2>&1 || continue
        patchelf --set-interpreter $(cat $NIX_CC/nix-support/dynamic-linker) "$toolchain_file" || true
	done
}

gcc_ver=15

echo ""
echo "[*] GCC $gcc_ver Toolchain setup!"
if [[ "$env_platform" = "mingw32" ]]; then
	toolchain_type="-w64-"
	toolchain_extension="zip"
else
	toolchain_type="-unknown-"
	toolchain_extension="tar.xz"
fi

if [ ! -f cache/toolchain.$toolchain_extension ]; then
	echo "GCC Toolchain cache download"
	curl -o cache/toolchain.$toolchain_extension -L -O https://github.com/AmanoTeam/obggcc/releases/download/gcc-$gcc_ver/$env_arch$toolchain_type$env_platform.$toolchain_extension
else
	echo "GCC Toolchain cache already exists, continuing"
fi

if [ ! -d toolchain/bin ]; then
	echo "GCC $gcc_ver Toolchain unpack..."
	bsdtar -xf cache/toolchain.$toolchain_extension --strip-components=1 -C $PWD/toolchain \
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

	if [[ "$env_distro" = *"nixos"* ]]; then
		echo "Patching Toolchain to run on NixOS"
		patch_toolchain_nixos "toolchain/bin/*"
		patch_toolchain_nixos "toolchain/libexec/gcc/arm-unknown-linux-gnueabi/$gcc_ver/*"
		patch_toolchain_nixos "toolchain/libexec/gcc/arm-unknown-linux-gnueabi/$gcc_ver/install-tools/*"
		patch_toolchain_nixos "toolchain/libexec/gcc/arm-unknown-linux-gnueabi/$gcc_ver/plugin/*"
		patch_toolchain_nixos "toolchain/arm-unknown-linux-gnueabi/bin/*"
	fi

	#echo "GCC Toolchain linking"
	#ln -s $PWD/toolchain/bin/arm-unknown-linux-gnueabi-as toolchain/bin/as

	echo "GCC Shebang fix"
	if [ "$env_platform" == "apple-darwin" ]; then
		sed -i '' "s|bin\/bash|usr\/bin\/env bash|" toolchain/bin/arm-unknown-linux-gnueabi2.13-pkg-config
	else
		sed -i "s|bin\/bash|usr\/bin\/env bash|" toolchain/bin/arm-unknown-linux-gnueabi2.13-pkg-config
	fi
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
	if [[ "$env_distro" = *"nixos"* ]]; then
		echo "XCB client NixOS build"
		nix build xcb/
	else
		echo "XCB client setup"
		python3 -m venv xcb
		source xcb/bin/activate
		python3 -m pip install pyserial
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
