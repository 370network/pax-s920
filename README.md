# ProlinOS/PAX toolchain build environment
An All-In-One toolchain build environment suite for use with the PAX S920 payment terminals running the ProlinOS Linux-based operating systen.
Beware that, by using this toolchain, you are already more than well aware of all required pre-requisites like the root process or other dependencies.

## setup instructions
either download the repo from [this link](https://github.com/370network/pax-s920/archive/refs/heads/main.zip) and unpack it or just git clone if you already have git installed.

### Debian, Ubuntu, Fedora, Alpine, postmarketOS, Arch and macOS
our toolchain script includes all necessary checks to make sure you have all things needed, just run 

```bash
./setup_toolchain.sh
```

### NixOS
NixOS wants to be special, so let's let it be so. Just run

```bash
nix-shell
./setup_toolchain.sh
```

### Windows
**build environment on Windows is supported, but highly experimental!**

to get working directly on Windows, just open the **`windows.bat`** batch script file. The same applies for opening the build environment later on as well.
Beware that you will have to run it multiple times, but it essentially combines everything the other systems need in an all in one solution.

### common for all other platforms
make sure you have `gcc`, `python3`, `python3-venv`, `git`, `swig`, `openssl` + devel, `qemu-user-static` installed and or their equivalents. Beware, that you are on your own as other platforms do not get any higher level of support.

```bash
./setup_toolchain.sh
```

## switching to the the build environment

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

you will be once again asked to either write the whole ***/dev/*** path of the PAX or the IP/hostname with the :5555 port

### paxdeployssh
a handy command to install your public keys from ~/.ssh or from a specified file path
```bash
paxdeployssh 
# or if you want to specify a file as well
paxdeployssh ~/.ssh/id_ed25519.pub
```

### paxpush
used as a shorter alias for the xcb command
```bash
paxpush <local source 1> <local source 2> .. <pax destination>
#eg. paxpush osal_test.c /data/app/MAINAPP/libosal.so
```

### paxpull
used as a shorter alias for the xcb command
```bash
paxpull <pax source> [optional: local destination]
#eg. paxpull /usr/lib/libcbinder.so libcbinder.so
```

### paxls
used as a shorter alias for the xcb command
```bash
paxls <pax directory>
#eg. paxls /data/app/MAINAPP
```

### paxdump
used as a shorter alias for the xcb command
```bash
paxdump <dump name> [optional: device path to dump]
#eg. paxdump mydump
```

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
