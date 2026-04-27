# ProlinOS/PAX toolchain
An All-In-One toolchain suite for use with the PAX S920 payment terminals running the ProlinOS Linux-based operating systen.
Beware that, by using this toolchain, you are already more than well aware of all required pre-requisites like the root process.

## setup instructions
### Debian, Ubuntu, Fedora, Alpine, postmarketOS
our toolchain script includes all necessary checks to make sure you have all things needed, just run 

```bash
./setup_toolchain.sh
```

### common for all other platforms
make sure you have `gcc`, `python3`, `python3-venv`, `git`, `swig`, `openssl` + devel, `qemu-user-static` installed and or their equivalents. Beware, that you are on your own as other platforms do not get any higher level of support.

```bash
./setup_toolchain.sh
```

## switching to the the toolchain environment

continue *(or start on your next session)* with
```bash
source env.sh
```

## commands
### paxreconfigure
in case you plugged your terminal over USB or the IP/hostname changed, run this command to change the XCB settings
```bash
paxreconfigure
```
*eg. paxreconfigure*

you will be once again asked to either write the whole ***/dev/*** path of the PAX or the IP/hostname with the :5555 port

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

### paxls
used as a shorter alias for the xcb command
```bash
paxls <pax directory>
```
*eg. paxls /data/app/MAINAPP*

### paxdump
used as a shorter alias for the xcb command
```bash
paxdump <dump name>
```
*eg. paxdump mydump*

The resulting dump will be stored in `dumps/<dump name>` inside the working directory.

## building homebrew
~~the official way of distributing packages is using the handy app package manager~~
~~download using the homebrew downloader (coming soon or never)~~

**for building homebrew:**
traverse into the `homebrew` folder, pick your desired port and run `./build.sh` or `make`


## additional official docs
see [pax-docs](https://github.com/370network/pax-s920/tree/main/pax-docs)

## headers
see [toolchain/paxroot/include](https://github.com/370network/pax-s920/tree/main/toolchain/paxroot/include)
