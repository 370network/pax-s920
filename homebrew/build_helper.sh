#build helpers
BUILD_FOLDER=$(pwd)

build_check () {
        if [ -f repo/$1 ]; then
                echo "$2 built successfully!"
                mv repo/$1 $2.so
                echo "file: $2.so"
        fi
}

build_dir_check () {
        if [ -d $1 ]; then
                echo "$2 built successfully!"
                mv $1 $2
                echo "directory: $2"
        fi
}

build_patch () {
	echo "applying patch $1..."

	if [ -f "$1.patched" ]; then
		echo "patch $1 already applied!"
		return
	fi

	patch -d $2 -p1 < $1

	if [ $? -eq 0 ]; then
		echo "successfully applied patch $1"
		touch "$1.patched"
	else
		echo "failed to apply patch $1"
	fi
}
