#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <errno.h>
#include <osal.h>

#include "xui.h"
#include "app.h"
#include "applist.h"
#include "ui.h"
#include "apploader.h"

void scan_dir_apps(AppList *list, const char* base_path) {
    char apps_path[512];
    
    if (access(base_path, F_OK) != 0) {
        printf("Base app folder '%s' does not exist\n", base_path);
        return;
    }
    
    snprintf(apps_path, sizeof(apps_path), "%s/apps/", base_path);
    printf("Scanning app folder '%s'...\n", apps_path);
    if (access(apps_path, F_OK) != 0) {
        printf("App folder '%s' does not exist\n", apps_path);
        return;
    }
    
    DIR *dir = opendir(apps_path);
    if (dir == NULL) {
        printf("Failed to open app folder");
    }

    // read all directories in apps_path
    struct dirent *entry;
    int is_empty = 1;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            is_empty = 0;
            printf("Found app: %s\n", entry->d_name);
            AppMetadata meta = parseFile(apps_path, entry->d_name);

            if (meta.is_valid) { 
                printf("App metadata parsed successfully.\n");
                printf("App: %s (v%s) by %s\n", meta.name, meta.version, meta.author);
                addApp(list, &meta);                
            } else {
                printf("Failed to parse app metadata, skipping it\n");
                printf("App: %s (v%s) by %s\n", meta.name, meta.version, meta.author);
                printf("Reason: %s\n", meta.is_valid ? "Valid" : "Invalid metadata"); 
            }
        }
    }

    if (is_empty) {
        printf("No apps found.\n");
    }
    closedir(dir);
}

int _init()
{
    printf("Pax Launcher v.1.0\n");
    
    //Load libosal stuff
    ui_funcs funcs;
    void *libosal = dlopen("/usr/lib/libosal.so", RTLD_LAZY);
    if (libosal) {
        funcs.OsSleep = dlsym(libosal, "OsSleep");
        funcs.OsSysSleepEx = dlsym(libosal, "OsSysSleepEx");
        funcs.OsCheckPowerSupply = dlsym(libosal, "OsCheckPowerSupply");
        funcs.OsMount = dlsym(libosal, "OsMount");
        funcs.OsUmount = dlsym(libosal, "OsUmount");
    }
    
    AppList list;
    initAppList(&list);

    // Handle /data/app/MAINAPP/apps/
    if (access("/data/app/MAINAPP/apps/", F_OK) != 0) {
        printf("App folder does not exist, attempting to create one\n");
        if (mkdir("/data/app/MAINAPP/apps/", 0777) != 0) {
            printf("Failed to create app folder: %s\n", strerror(errno));
        }
    }
    
    scan_dir_apps(&list, "/data/app/MAINAPP");

    if (access("/mnt/sdcard", F_OK) != 0) {
        mkdir("/mnt/sdcard", 0777);
        printf("Created /mnt/sdcard\n");
    }

    int ret = funcs.OsMount("/dev/block/mmcblk0p1", "/mnt/sdcard", "vfat", 0, 0);
    if (ret == -1003) {
        printf("The SD card is already mounted or there's a different problem!\n");
    } else if (ret != 0) {
        printf("Mounting SD card failed! Error: %d\n", ret);
    }
    
    scan_dir_apps(&list, "/mnt/sdcard");

    printf("Enumerating all apps in the list:\n");
    for (int i = 0; i < list.count; i++) {
        AppMetadata *app = &list.apps[i];
        printf("App %d: %s (v%s) by %s\n", i + 1, app->name, app->version, app->author);
    }

    printf("Initializing UI...\n");
    int running = 1;
    while (running) {
        switch (initui(&funcs, &list)) {
            case UI_RESULT_RELAUNCH:
                printf("Result: relaunch\n");
                funcs.OsSleep(500); //In case a key is pressed when app exited
                XuiClearKey();
                break;
            case UI_RESULT_EXIT:
                printf("Result: exiting\n");
                running = 0;
                break;
        }
    }

    funcs.OsUmount("/mnt/sdcard", 0);

    exit(0);
}
