with (import <nixpkgs> {});
mkShell {
	buildInputs = [
		curl
		git
		m4
		autoconf
		automake
		cmake
		pkgconf
		openssl
		python3
		swig
	];
	
}
