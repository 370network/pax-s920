#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <xui.h>
#include "applist.h"
    
typedef struct {
    void (*OsSleep)(unsigned int);
    int (*OsSysSleepEx)(int);
    int (*OsCheckPowerSupply)(void);
    int (*OsMount)(char*, char*, char*, int, const void*);
    int (*OsUmount)(char*, int);
} ui_funcs;

typedef enum {
    UI_RESULT_NONE = 0,
    UI_RESULT_RELAUNCH = 1,
    UI_RESULT_EXIT = 2
} UIResult;

UIResult initui(ui_funcs *funcs, AppList *applist);

#ifdef __cplusplus
}
#endif

#endif // UI_MANAGER_H
