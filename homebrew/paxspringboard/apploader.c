#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h> 

int LoadApp(const char *app_path, const char *appName) {
    void *handle;
    char binPath[512];
    char *argv[2];
    pid_t forked_pid;
    int status = 0;
    printf("Loading app: %s\n", appName);
    
    //We need to fork the process, otherwise doing dlopen the second time will not reload the lib as is already resident
    forked_pid = fork();
    if (forked_pid == 0) {
        //Forked process, launch lib

        //Set current dir
        chdir(app_path);

        snprintf(binPath, sizeof(binPath), "%s/%s", app_path, appName);
        if (access(binPath, F_OK) == 0) {
            //execv into binary, call shouldn't return on success
            argv[0] = binPath;
            argv[1] = NULL;
            status = execv(argv[0], argv);
            printf("execv failed: %d\n", status);
        } else {
            //dlopen the so
            snprintf(binPath, sizeof(binPath), "%s/%s.so", app_path, appName);
            if (access(binPath, F_OK) != 0) {
                printf("Not found any executable or library at: %s\n", app_path);
                exit(1);
            }
            handle = dlopen(binPath, RTLD_LAZY | RTLD_LOCAL);
            if (!handle) {
                printf("dlopen failed: %s\n", dlerror());
                exit(1);
            }
            dlclose(handle);
        }
        
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
