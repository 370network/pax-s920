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


    printf("Enumerating all apps in the list:\n");
    for (int i = 0; i < list.count; i++) {
        AppMetadata *app = &list.apps[i];
        printf("App %d: %s (v%s) by %s\n", i + 1, app->name, app->version, app->author);
    }

    printf("Initializing UI...\n");
    initui(&list);

    sleep(10);


    exit(0);
}