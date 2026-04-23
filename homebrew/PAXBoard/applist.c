#include "applist.h"
#include "app.h"
#include <stdlib.h>
#include <stdio.h>

void initAppList(AppList* list) {

    list->apps = malloc(sizeof(AppMetadata) * 1);
    if (list->apps == NULL) {
        printf("malloc failed");
    }
    list->count = 0;
    list->capacity = 1;
}

void addApp(AppList* list, const AppMetadata* app) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        AppMetadata* newApps = realloc(list->apps, sizeof(AppMetadata) * list->capacity);
        if (newApps == NULL) {
            printf("realloc failed");
        }
        list->apps = newApps;
    }

    list->apps[list->count] = *app;
    list->count++;
}

AppMetadata* getApps(const AppList* list, size_t* count) {
    if (count != NULL) {
        *count = list->count;
    }
    return list->apps;
}

void freeAppList(AppList* list) {
    free(list->apps);
    list->apps = NULL;
    list->count = 0;
    list->capacity = 0;
}