# ProlinOS/PAX toolchain

## setup instructions
### debian/ubuntu 64bit specific
install multiarch support
```bash
sudo dpkg --add-architecture i386
sudo apt-get update
sudo apt-get install libc6:i386 libncurses5:i386 libstdc++6:i386
```

### common for all platforms
make sure you have `python3`, `python3-venv`, `wget`, `git`, `swig`, `openssl`+devel *(`qemu-user-static`/`qemu-arm-static` for possibly running executables)* installed

```bash
./setup_toolchain.sh
```

continue *(or start on your next session)* with
```bash
source env.sh
```

## commands
### paxpush
used as a shorter alias for the xcb command
```bash
paxpush <source file> <pax destination>
```
*eg. paxpush osal_test.c /data/app/MAINAPP/libosal.so*

### paxpull
used as a shorter alias for the xcb command
```bash
paxpull <pax source> <local destination>
```
*eg. paxpull /usr/lib/libcbinder.so libcbinder.so*

## building homebrew
~~download using the homebrew downloader (coming soon or never)~~

traverse into the `homebrew` folder, pick your desired port and run `./install.sh`, `./build.sh` or `make`

some ports (mainly libs)* contain both `./install.sh` and `./build.sh` because you can save time by just installing it instead of building them

## additional official docs
see [pax-docs](https://github.com/370network/pax-s920/tree/main/pax-docs)

## headers

### xui.h
half baked and handmade header file for nearly all libxui.so relevant functions. For (partial) usage, look at xui_test.c

### osal.h
header file for all libosal.so relevant functions. For usage, look at osal_test.c

### prolin-printf.h
header file use as redefinition of printf with flush to stdout.

`-include prolin-printf.h` in cflagoch for global usage across projects
