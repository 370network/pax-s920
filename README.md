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
make sure you have `python3`, `python3-venv`, `wget`, `git`, *(`qemu-user-static`/`qemu-arm-static` for possibly running executables)* installed

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
poloprepečený a ručne vyskladaný header pre skoro šecky libxui.so relevantné funkcie, pre čiastočné použití kukaj xui_test.c

### osal.h
header pre šecky libosal.so relevantné funkcie, pre použití kukaj osal_test.c

### prolin-printf.h
header použitý ako redefinícia pre printf s flushom do stdoutu.

`-include prolin-printf.h` v cflagoch aby sa globálne natlačil na všetky zdrojáky v projekte
