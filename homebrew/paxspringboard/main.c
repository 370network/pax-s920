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


int _init()
{
    printf("Pax Launcher v.1.0\n");
    printf("Checking if app folder exists...\n");

    // /data/app/MAINAPP/apps/
    if (access("/data/app/MAINAPP/apps/", F_OK) == 0) {
        printf("App folder exists.\n");
    } else {
        printf("App folder does not exist, attempting to create one\n");
        if (mkdir("/data/app/MAINAPP/apps/", 0755) == 0) {
            printf("App folder created successfully.\n");
        } else {
            printf("Failed to create app folder: %s\n", strerror(errno));
        }
    }

    printf("Checking if app folder is empty...\n");
    DIR *dir = opendir("/data/app/MAINAPP/apps/");
    if (dir == NULL) {
        printf("Failed to open app folder");
    }
    
    AppList list;
    initAppList(&list);

    // read all directories in /data/app/MAINAPP/apps/
    struct dirent *entry;
    int is_empty = 1;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            is_empty = 0;
            printf("Found app: %s\n", entry->d_name);
            AppMetadata meta = parseFile(entry->d_name);

            if (meta.is_valid) { 
                printf("App metadata parsed successfully.\n");
                printf("App: %s (v%s) by %s\n", meta.name, meta.version, meta.author);
                addApp(&list, &meta);
                
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


    void *libosal = dlopen("/usr/lib/libosal.so", RTLD_LAZY);

    void (*OsSleep)(unsigned int) = dlsym(libosal, "OsSleep");
    int (*OsSysSleepEx)(int) = dlsym(libosal, "OsSysSleepEx");
    int (*OsCheckPowerSupply)(void) = dlsym(libosal, "OsCheckPowerSupply");

    printf("Enumerating all apps in the list:\n");
    for (int i = 0; i < list.count; i++) {
        AppMetadata *app = &list.apps[i];
        printf("App %d: %s (v%s) by %s\n", i + 1, app->name, app->version, app->author);
    }

    printf("Initializing UI...\n");
    int running = 1;
    while (running) {
        switch (initui(&list)) {
            case UI_RESULT_RELAUNCH:
                printf("Result: relaunch\n");
                OsSleep(500); //In case a key is pressed when app exited
                XuiClearKey();
                break;
            case UI_RESULT_EXIT:
                printf("Result: exiting\n");
                running = 0;
                break;
            case UI_RESULT_SUSPEND:
                int supply = OsCheckPowerSupply();
                printf("Result: suspend - power supply: %d\n", supply);
                if (supply == POWER_BATTERY) {
                    //Deep sleep
                    OsSysSleepEx(2);
                } else {
                    //Plugged in so just shut the screen, or will wake up again
                    OsSysSleepEx(1);
                }
                break;
        }
    }

    exit(0);
}