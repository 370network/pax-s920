#include <dlfcn.h>
#include <stdlib.h>

// lib symbol loader
typedef void (*generic_func)();
generic_func load_symbol(void *lib_handle, const char *symbol_name) {
    generic_func symbol = (generic_func)dlsym(lib_handle, symbol_name);
    return symbol;
}

int _init()
{
        void *libosal = dlopen("/usr/lib/libosal.so", RTLD_LAZY);

        void (*OsScrBrightness)() = load_symbol(libosal, "OsScrBrightness");

        int i;
        for(i = 0; i < 10; i++)
        {
                printf("Setting screen brightness to: %d\n", i);
                OsScrBrightness(i);
                sleep(1);
        }

        return(0);
}


