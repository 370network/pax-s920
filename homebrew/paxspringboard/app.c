#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include "app.h"

AppMetadata parseFile(const char *apps_path, const char *appName) {
    AppMetadata app = {0};
    app.icon = 0;
    app.is_valid = false;
    char autolaunch_str[5];
    strcpy(autolaunch_str, "0");

    printf("Parsing application: %s\n", appName);

    strncpy(app.app, appName, sizeof(app.app) - 1);
    app.app[sizeof(app.app) - 1] = '\0';

    snprintf(app.path, sizeof(app.path), "%s/%s", apps_path, appName);
    app.app[sizeof(app.path) - 1] = '\0';
    printf("Base path: %s\n", app.path);

    char infoPath[512];
    snprintf(infoPath, sizeof(infoPath), "%s/info.txt", app.path);

    char iconPath[512];
    snprintf(iconPath, sizeof(iconPath), "%s/icon.png", app.path);

    const char *required_files[][2] = {
        {iconPath, "icon"},
        {infoPath, "info"}};
    size_t num_required_files = sizeof(required_files) / sizeof(required_files[0]);

    for (size_t i = 0; i < num_required_files; ++i) {
        if (access(required_files[i][0], F_OK) != 0) {
            printf("ERROR: Application '%s' is missing %s file.\n", appName, required_files[i][1]);
            return app;
        }
    }

    char binPath[512];
    snprintf(binPath, sizeof(binPath), "%s/%s.so", app.path, appName);
    if (access(binPath, F_OK) != 0) {
        snprintf(binPath, sizeof(binPath), "%s/%s", app.path, appName);
        if (access(binPath, F_OK) != 0) {
            printf("ERROR: Application '%s' is missing the binary, can be %s library or %s executable.\n", appName, binPath);
            return app;
        }
    }

    FILE *file = fopen(infoPath, "r");
    if (!file) {
        printf("ERROR: Cannot open metadata for '%s': %s\n", appName, strerror(errno));
        return app;
    }

    typedef struct {
        const char *key;
        char *value;
        int size;
    } MetadataParser;

    MetadataParser parsers[] = {
        {"name:", app.name, sizeof(app.name)},
        {"version:", app.version, sizeof(app.version)},
        {"author:", app.author, sizeof(app.author)},
        {"description:", app.description, sizeof(app.description)},
        {"license:", app.license, sizeof(app.license)},
        {"url:", app.url, sizeof(app.url)},
        {"autolaunch:", autolaunch_str, sizeof(autolaunch_str)},
    };
    size_t num_parsers = sizeof(parsers) / sizeof(parsers[0]);

    char line[512];
    while (fgets(line, sizeof(line), file)) {
        for (size_t i = 0; i < num_parsers; ++i) {
            size_t key_len = strlen(parsers[i].key);
            if (strncmp(line, parsers[i].key, key_len) == 0) {
                char format[32];
                snprintf(format, sizeof(format), " %%%d[^\n]", parsers[i].size - 1);
                sscanf(line + key_len, format, parsers[i].value);
                break;
            }
        }
    }
    fclose(file);

    app.autolaunch = atoi(autolaunch_str);

    const char *required_fields[] = {app.name, app.version, app.author, app.description};
    size_t num_required_fields = sizeof(required_fields) / sizeof(required_fields[0]);

    for (size_t i = 0; i < num_required_fields; ++i) {
        if (strlen(required_fields[i]) == 0) {
            printf("ERROR: Missing required metadata fields in '%s/info.txt'\n", appName);
            return app;
        }
    }

    app.is_valid = true;

    printf("Parsed app as '%s':\n", appName);
    printf("Path: %s\n", app.path);
    printf("Name: %s\n", app.name);
    printf("Version: %s\n", app.version);
    printf("Author: %s\n", app.author);
    printf("Description: %s\n", app.description);
    printf("License: %s\n", app.license);
    printf("URL: %s\n", app.url);
    printf("Autolaunch: %d\n", app.autolaunch);
    printf("App is valid: %s\n", app.is_valid ? "true" : "false");

    return app;
}
