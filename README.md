# ProlinOS/PAX toolchain

## setup instructions
make sure you have python3 venv, wget, git, *(qemu-user-static/qemu-arm-static for possibly running executables)* installed
```bash
./setup_toolchain.sh
```

continue with
```bash
source xui/bin/activate
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


## additional official docs
[pax-docs](https://github.com/370network/pax-s920/tree/main/pax-docs)

## headers

### xui.h
poloprepečený a ručne vyskladaný header pre skoro šecky libxui.so relevantné funkcie, pre čiastočné použití kukaj xui_test.c

### osal.h
header pre šecky libosal.so relevantné funkcie, pre použití kukaj osal_test.c

### prolin-printf.h
header použitý ako redefinícia pre printf s flushom do stdoutu.

`-include prolin-printf.h` v cflagoch aby sa globálne natlačil na všetky zdrojáky v projekte
