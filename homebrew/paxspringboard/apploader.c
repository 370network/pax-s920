#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h> 

int LoadApp(const char *appName) {
    void *handle;
    char app_path[256];
    char so_path[256];
    pid_t forked_pid;
    int status = 0;
    printf("Loading app: %s\n", appName);
    
    //We need to fork the process, otherwise doing dlopen the second time will not reload the lib as is already resident
    forked_pid = fork();
    if (forked_pid == 0) {
        //Forked process, launch lib

        //Set current dir
        snprintf(app_path, sizeof(app_path), "/data/app/MAINAPP/apps/%s", appName);
        chdir(app_path);

        //dlopen the so
        snprintf(so_path, sizeof(so_path), "%s/%s.so", app_path, appName);
        handle = dlopen(so_path, RTLD_LAZY | RTLD_LOCAL);
        if (!handle) {
            printf("dlopen failed: %s\n", dlerror());
            return 1;
        }
        dlclose(handle);
        
        //Terminate the forked process
        exit(0);
    } else if (forked_pid > 0) {
        waitpid(forked_pid, &status, 0);
        printf("App PID %d status %d\n", forked_pid, status);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);        
            if (exit_status != 0) {
                printf("App exited with %d code\n", exit_status);
            }
        } else {
            printf("App terminated unexpectly\n");
        }
    } else {
        printf("Error: fork failed %d\n", forked_pid);
    }
    
    return 0;
}
