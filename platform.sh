#!/usr/bin/env bash
# platform.sh - Morc @ 370network
# middleware identification boilerplate used in other scripts

env_platform=$(uname)
env_libc=$(ldd --version 2>&1)
if [ "$env_platform" == "Linux" ]; then
	if [[ "$env_libc" == *"musl"* ]]; then
		env_platform="linux-musl"
	else
		env_platform="linux-gnu"
	fi
elif [ "$env_platform" == "Darwin" ]; then
	env_platform="apple-darwin"
elif [[ "$env_platform" == *"MINGW"* ]]; then
	env_platform="mingw32"
fi

env_distro="generic"
if [ "$env_platform" != "apple-darwin" ]; then
	if [ -f /etc/os-release ]; then
		. /etc/os-release
		env_distro=$ID
	elif [ -f /etc/lsb-release ]; then
		. /etc/lsb-release
		env_distro=$ID
	fi
	env_distro="${env_distro,,}"
fi

env_arch=$(uname -m)
if [ "$env_arch" == "arm64" ]; then
	env_arch="aarch64"
fi

echo "export env_platform=$env_platform env_distro=$env_distro env_arch=$env_arch"