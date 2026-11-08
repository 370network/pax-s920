with (import <nixpkgs> {});
mkShell {
	buildInputs = [
		curl
		git
		m4
		autoconf
		automake
		libtool
		cmake
		pkgconf
		openssl
		python3
		libarchive
		qemu-user
	];
	
}
