#ifndef APPLIST_H
#define APPLIST_H

#include <stddef.h>
#include "app.h"

// Define a struct to represent the AppList
typedef struct {
    AppMetadata* apps; 
    size_t count;     
    size_t capacity;   
} AppList;

void initAppList(AppList* list);
void addApp(AppList* list, const AppMetadata* app);
AppMetadata* getApps(const AppList* list, size_t* count);
void freeAppList(AppList* list);

#endif // APPLIST_H