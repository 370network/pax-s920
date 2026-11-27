#build helpers
_pkg_log_info() {
	printf '[*] %s\n' "$*"
}

_pkg_log_success() {
	printf '[+] %s\n' "$*"
}

_pkg_log_skip() {
	printf '[-] %s\n' "$*"
}

_pkg_log_error() {
	printf '[!] %s\n' "$*" >&2
}

_pkg_name_sanitize() {
	printf '%s\n' "$1" | LC_ALL=C sed -e 's/[^a-zA-Z0-9._+-][^a-zA-Z0-9._+-]*/-/g' -e 's/^[^a-zA-Z0-9]*//' -e 's/[._+-]*$//'
}

build_clean()  {
	for _pkg_dir in */; do
		rm -rf "$_pkg_dir"*.so
		rm -rf "$_pkg_dir"*.o
		rm -rf "$_pkg_dir"*.apk
	done
	rm -rf *.so
	rm -rf *.o
	rm -rf *.apk
	rm -rf *.tar.gz
	rm -rf *.tar.xz
	rm -rf pkg_output repo
}

build_header() {
	printf '\n%s\n' '========================================'
	printf        '           370network paxdevs\n\n'
	printf        "  Building package: $PKG_NAME\n"
	printf        "  Description: $PKG_DESC\n"
	printf        "  Version: $PKG_VER\n"
	printf        "  Author: $PKG_AUTHOR\n"
	if [ -n "${PKG_DEPENDS-}" ]; then
		printf    "  Dependencies: $PKG_VER\n"
	fi
	if [ -n "${PKG_TAGS-}" ]; then
		printf    "  Tags: $PKG_TAGS\n"
	fi
	if [ -n "${PKG_URL-}" ]; then
		printf    "  URL: $PKG_URL\n"
	fi
	if [ -n "${PKG_LICENSE-}" ]; then
		printf    "  License: $PKG_LICENSE\n"
	fi
	printf '\n%s\n' '========================================'
	printf '\n'
}

build_patch () {
	_pkg_log_info "Applying patch $1..."

	if [ -f "$2/${1##*/}.patched" ]; then
		_pkg_log_skip "Patch $1 already applied!"
		return
	fi

	patch -d $2 -p1 < $1

	if [ $? -eq 0 ]; then
		_pkg_log_success "Successfully applied patch $1"
		touch "$2/${1##*/}.patched"
	else
		_patching_status=$?
		_pkg_log_error "Failed to apply patch $1: $_patching_status"
		exit "$_patching_status"
	fi
}

build_apply_all_patches () {
	for patch_file in "patches"/*.patch; do
		build_patch $patch_file "$1"
	done
}

build_dependency() {
	_pkg_dependency=$1
	
	_pkg_log_info "Building dependency: $_pkg_dependency"
	
	if (cd "../$_pkg_dependency" && _BUILD_MODE=dependency sh build.sh); then
		_pkg_log_success "Dependency $_pkg_dependency completed!"
	else
		_pkg_dependency_status=$?
		_pkg_log_error "Dependency failed: $_pkg_dependency"
		exit "$_pkg_dependency_status"
	fi
}

build() {
	BUILD_FOLDER=$(pwd)
	
	_pkg_log_info "Building $PKG_NAME"

	if ( _build ); then
		_pkg_log_success "Building completed!"
	else
		_build_status=$?
		_pkg_log_error "Build failed: $_build_status"
		exit "$_build_status"
	fi
}


packaging() {
	_pkg_log_info "Packaging $PKG_NAME"

	if ( _packaging ); then
		_pkg_log_success "Packaging completed!"
	else
		_packaging_status=$?
		_pkg_log_error "Packaging failed: $_packaging_status"
		exit "$_packaging_status"
	fi
}

package_init() {
	PKG_BASE_DIR='.pkg_stage'
	PKG_APPS_DIR='.pkg_stage/apps'
	PKG_LIB_DIR='.pkg_stage/lib'
	PKG_BIN_DIR='.pkg_stage/bin'
	
	if ! mkdir -p "pkg_output"; then
		_pkg_log_error "Failed to create pkg_output"
		return 1
	fi
	
	#sanitize name for use in later commands
	_pkg_name=$(_pkg_name_sanitize "$PKG_NAME")

	_pkg_log_info 'Preparing package staging directories'
	if [ "${PKG_ISAPP-0}" = '1' ]; then

		PKG_APP_DIR="$PKG_APPS_DIR/$_pkg_name"
		
		if ! mkdir -p "$PKG_APPS_DIR" "$PKG_LIB_DIR" "$PKG_BIN_DIR" "$PKG_APP_DIR"; then
			_pkg_log_error 'Could not initialize package staging directories'
			return 1
		fi
	else
		unset PKG_APP_DIR

		if ! mkdir -p "$PKG_LIB_DIR" "$PKG_BIN_DIR"; then
			_pkg_log_error 'Could not create package directories'
			return 1
		fi
	fi
}

package_create() {
	_PKG_APK_CREATED=0

	_pkg_output_file="./$_pkg_name-$PKG_VER.apk"

	if [ "${PKG_ISAPP-0}" = '1' ]; then
		PKG_APPS_DIR=${PKG_APPS_DIR:-pkg_output/apps}

		if ! {
			printf 'name: %s\n' "$PKG_NAME" &&
			printf 'description: %s\n' "$PKG_DESC" &&
			printf 'version: %s\n' "$PKG_VER" &&
			printf 'author: %s\n' "$PKG_AUTHOR" &&
			{ [ -z "${PKG_LICENSE-}" ] || printf 'license: %s\n' "$PKG_LICENSE"; } &&
			{ [ -z "${PKG_URL-}" ] || printf 'url: %s\n' "$PKG_URL"; }
		} > "$PKG_APP_DIR/info.txt"; then
			_pkg_log_error "Could not write info.txt!"
			unset _pkg_output_file
			return 1
		fi
	fi

	#_pkg_log_info 'Cleaning unused empty directories'
	for _pkg_dir in .pkg_stage/*; do
		if [ -d "$_pkg_dir" ]; then
			rmdir "$_pkg_dir" 2>/dev/null || :
		fi
	done

	if ! command -v apk.static >/dev/null 2>&1; then
		_pkg_log_skip "No packaging tools installed, won't proceed to create an apk package!"
	else
		_pkg_log_info "Creating package: $_pkg_output_file"

		set -- apk.static mkpkg \
			--info arch:armel \
			--files ./.pkg_stage \
			--info "name:$_pkg_name" \
			--info "version:$PKG_VER" \
			--info "description:$PKG_DESC" \
			--info "maintainer:$PKG_AUTHOR"

		if [ -n "${PKG_DEPENDS-}" ]; then
			set -- "$@" --info "depends:$PKG_DEPENDS"
		fi

		if [ -n "${PKG_TAGS-}" ]; then
			set -- "$@" --info "tags:$PKG_TAGS"
		fi

		if [ -n "${PKG_LICENSE-}" ]; then
			set -- "$@" --info "license:$PKG_LICENSE"
		fi

		if [ -n "${PKG_URL-}" ]; then
			set -- "$@" --info "url:$PKG_URL"
		fi

		set -- "$@" --output "$_pkg_output_file"

		if "$@"; then
			_PKG_APK_CREATED=1
			_pkg_log_success "Created $_pkg_output_file"
		else
			_pkg_status=$?
			_pkg_log_error "Package creation failed! (exit status $_pkg_status)"
			unset _pkg_name _pkg_output_file _pkg_dir
			return "$_pkg_status"
		fi
	fi

	if ! cp -RP ".pkg_stage/." "pkg_output/"; then
		_pkg_log_error 'Failed to mirror staging into pkg_output'
		unset _pkg_name _pkg_output_file _pkg_dir
		return 1
	fi

	if ! rm -rf ".pkg_stage"; then
		_pkg_log_error 'Failed to clean staging'
		unset _pkg_name _pkg_output_file _pkg_dir
		return 1
	fi

	if [ "${_BUILD_MODE-}" != 'dependency' ]; then
		printf '\n'
		_pkg_log_info 'You can now push the content of the pkg_output folder to your device:'
		printf    '    paxpush pkg_output/* /data/app/MAINAPP/\n'
		printf    "      or to the device's SD card:\n"
		printf    '    paxpush pkg_output/* /mnt/sdcard/\n'
		printf '\n'
		
		if command -v apk.static >/dev/null 2>&1; then
		_pkg_log_info 'Or you can copy the built apk packages to your device:'
		printf    '    paxpush *.apk /data/app/MAINAPP/\n'
		printf    "      or to the device's SD card:\n"
		printf    '    paxpush *.apk /mnt/sdcard/\n'
		fi
		
		printf '\n'
		_pkg_log_info 'Beware that installing packages manually comes with NO WARRANTY and by that point you must know what you are doing!'
	fi

	unset _pkg_name _pkg_output_file _pkg_dir _pkg_status
	return 0
}

wont_package() {
	printf '\n'
	_pkg_log_info 'The following package is already present on your device!'
	printf    '    You can use the installed build outputs for building other packages for your device.\n'
}