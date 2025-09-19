#ifndef APPLOADER_H
#define APPLOADER_H

#include <stdio.h>
#include <dlfcn.h>

// Typedef for the initialization function pointer
typedef void (*init_func_t)(void);

/**
 * @brief Loads and initializes a shared library application.
 * 
 * @param appName The name of the application (without the .so extension).
 */
void LoadApp(const char* app_path, const char *appName);

#endif // APPLOADER_H
