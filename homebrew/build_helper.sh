#build helpers
build_check () {
        if [ -f repo/$1 ]; then
                echo "$2 built successfully!"
                mv repo/$1 $2.so
                echo "file: $2.so"
        fi
}
