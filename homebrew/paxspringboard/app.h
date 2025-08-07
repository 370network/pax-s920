#ifndef APP_PARSER_H
#define APP_PARSER_H

#include <stdbool.h>

// Structure to hold application metadata
typedef struct {
    char app[256];
    char name[256];
    char version[32];
    char author[256];
    char description[512];
    char license[128];
    char url[256];
    bool is_valid;
} AppMetadata;

/**
 * @brief Parses the application metadata from a given application folder.
 *
 * This function reads the metadata of an application located in the
 * `/data/app/MAINAPP/apps/<appName>` directory. It verifies the presence
 * of required files (binary `.so`, `icon.png`, and `info.txt`) and
 * extracts metadata fields from `info.txt`.
 *
 * @param appName The name of the application to parse.
 * @return An AppMetadata structure filled with parsed data. If parsing fails,
 *         fields may remain empty strings.
 */
AppMetadata parseFile(const char *appName);

#endif // APP_PARSER_H
