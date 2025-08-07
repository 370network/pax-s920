#include <stdio.h>
#include <dlfcn.h>

int LoadApp(const char *appName) {
    printf("Loading app: %s\n", appName);
    void *handle;
    char *error;

    
    printf("stage 1...\n");
    char path[256];
    snprintf(path, sizeof(path), "/data/app/MAINAPP/apps/%s/%s.so", appName, appName);
    handle = dlopen(path, RTLD_LAZY);
    if (!handle) {
        printf("dlopen failed: %s\n", dlerror());
        return 1;
    }

    dlclose(handle);
    return 0;
}
