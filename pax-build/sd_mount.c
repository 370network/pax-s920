#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// lib symbol loader
typedef void (*generic_func)();
generic_func load_symbol(void *lib_handle, const char *symbol_name) {
    generic_func symbol = (generic_func)dlsym(lib_handle, symbol_name);
    return symbol;
}
//

int _init()
{
        void *libosal = dlopen("/usr/lib/libosal.so", RTLD_LAZY);

        int (*OsMount)() = load_symbol(libosal, "OsMount");

        struct stat st = {0};
        if (stat("/mnt/sdcard", &st) == -1) {
                mkdir("/mnt/sdcard", 0700);
                printf("Creating /mnt/sdcard\n");
        }

        int ret = OsMount("/dev/block/mmcblk0p1", "/mnt/sdcard", "vfat", 0, 0);
        if (ret != 0)
                if (ret == -1003)
                        printf("The SD card is already mounted or there's a different problem!\n");
                else
                        printf("Mounting FAILED! Error: %d\n", ret);
        else
                printf("Mounting was successful!");

        fflush(stdout);

        return(0);
}
