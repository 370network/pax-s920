#include "ui.h"
#include "app.h"
#include "applist.h"
#include <stdio.h>
#include <osal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <math.h>
#include "apploader.h"

const int GRID_W = 3;
const int GRID_H = 4;
const int GRID_MARGIN = 3;
const int GRID_BUTTONS = GRID_W * GRID_H;
const uint16_t GRID_KEYCODE = 0xE000;

#define BATT_STATUS_TEXT_LEN 16
static char battery_status_text_old[BATT_STATUS_TEXT_LEN];

typedef struct {
    ui_funcs *funcs;
    AppList *applist;
    XuiWindow *root;
    XuiFont *font;
    XuiWindow *statusbar;
    XuiWindow *statusbar_battery;
    size_t page;
} ui_state;

void clear_framebuffer() {
    int fd;
    void* framebuffer_mmap;
    struct fb_var_screeninfo vinfo;

    //Open framebuffer device
    fd = open("/dev/fb", O_RDWR);
    if (fd == -1) {
        perror("Failed to open framebuffer device");
        return;
    }

    //Get framebuffer dimensions
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        printf("Framebuffer ioctl FBIOGET_VSCREENINFO error\n");
        goto err_mmap;
    }
 
    //Compute the length of framebuffer
    size_t len = vinfo.xres * vinfo.yres * (vinfo.bits_per_pixel / 8);
    
    //Map it so we can write
    framebuffer_mmap = mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ((int) framebuffer_mmap == -1) {
        printf("Framebuffer mmap error: %d\n", (int) framebuffer_mmap);
        goto err_mmap;
    }
    
    memset(framebuffer_mmap, 0, len);
    
    munmap(framebuffer_mmap, len);

err_mmap:
    close(fd);
}

void destroyui(ui_state *state) {
    //Free up icons
    for (int i = 0; i < state->applist->count; i++) {
        XuiImgFree(state->applist->apps[i].icon);
        state->applist->apps[i].icon = 0;
    }
    if (state->statusbar_battery) {
        XuiDestroyWindow(state->statusbar_battery);
        state->statusbar_battery = 0;
    }
    state->statusbar = 0;
    state->root = 0;
    XuiClose();
}

int launch_app(ui_state *state, size_t button_index) {
    int app_index = state->page * GRID_BUTTONS + button_index;
    if (app_index < state->applist->count) {
        printf("Launching app: %d %s\n", app_index, state->applist->apps[app_index].name);
        destroyui(state);
        clear_framebuffer();
        LoadApp(state->applist->apps[app_index].path, state->applist->apps[app_index].app);
        clear_framebuffer(); //Clear after app ran
        return UI_RESULT_RELAUNCH;
    } else {
        return UI_RESULT_NONE;
    }
}

void draw_statusbar(ui_state *state, bool first) {
    char battery_status_text[BATT_STATUS_TEXT_LEN];
    char battery_level[BATT_STATUS_TEXT_LEN];
    char battery_state;
    FILE* file_battery_status;
    char *statusbar_text = "PaxBoard";
    uint32_t statusbar_bg_color = XuiColor(0, 128, 0, 0);
    
    // Battery and ethernet status in future? 
    // XuiSetStatusbarIcon(5, "/usr/share/tm/tm_icon_pageup.bmp");
	// XuiSetStatusbarIcon(7, "/usr/share/tm/tm_icon_pagedown.bmp");
    
    //Compute the battery text
    battery_state = (state->funcs->OsCheckPowerSupply() == POWER_BATTERY) ? ' ' : '+';
    file_battery_status = fopen("/sys/class/power_supply/battery/capacity","r");
    if (file_battery_status != NULL) {
    }
    memset(battery_level, 0, BATT_STATUS_TEXT_LEN);
    if (fgets(battery_level, 4, file_battery_status) == NULL) {
        strcpy(battery_level, "???");
    } else {
        char* c = battery_level;
        //Remove newline lol
        while (*c != 0) {
            if (*c == '\n') {
                *c = 0;
                break;
            }
            c++;
        }
    }
    fclose(file_battery_status);
    snprintf(battery_status_text, sizeof(battery_status_text), "%c%s%%", battery_state, battery_level);
    
    //Avoid updating too many times
    if (!first && strncmp(battery_status_text_old, battery_status_text, BATT_STATUS_TEXT_LEN) == 0) {
        return;
    }
    strncpy(battery_status_text_old, battery_status_text, BATT_STATUS_TEXT_LEN);
    const int BATT_TEXT_FONT_SIZE = 8;
    int batt_text_len = XuiTextWidth(state->font, BATT_TEXT_FONT_SIZE, battery_status_text);
    int batt_text_x = 240 - batt_text_len - 4;
    
    //printf("Battery: '%s'\n", battery_status_text);

    if (first) {
        XuiCanvasSetBackground(state->statusbar, XUI_BG_NORMAL, 0, statusbar_bg_color);
        XuiCanvasDrawText(state->statusbar, 5, 15, 10, state->font, 0, XuiColor(255, 255, 255, 255), statusbar_text);
    }
    if (state->statusbar_battery) {
        XuiDestroyWindow(state->statusbar_battery);
        state->statusbar_battery = 0;
    }
    
    state->statusbar_battery = XuiCreateCanvas(state->statusbar, batt_text_x, 0, batt_text_len, 32);
    XuiCanvasSetBackground(state->statusbar_battery, XUI_BG_NORMAL, 0, statusbar_bg_color);
    XuiCanvasDrawText(state->statusbar_battery, 0, 15, BATT_TEXT_FONT_SIZE, state->font, 0, XuiColor(255, 255, 255, 255), battery_status_text);
    XuiShowWindow(state->statusbar_battery, 1, 0);
}

void draw_grid(ui_state *state) {
    AppList *applist = state->applist;
    char icon_path[512];
    int cell_w = ceil(240.0 / (float) GRID_W);
    int cell_h = ceil(280.0 / (float) GRID_H);
    int icon_w = cell_w - GRID_MARGIN * 2;
    int icon_h = cell_h - GRID_MARGIN * 2;
    
    draw_statusbar(state, true);

    XuiCanvasSetBackground(state->root, XUI_BG_NORMAL, 0, XuiColor(255, 255, 255, 255));

    if (applist && applist->count > 0) {
        for (int bi = 0; bi < GRID_BUTTONS; bi++) {
            int i = state->page * GRID_BUTTONS + bi;
            if (i >= applist->count) {
                break;
            }
            AppMetadata *app = &applist->apps[i];
            if (app && app->name) {
                printf("Adding app %d: %s\n", i, applist->apps[i].name);

                snprintf(icon_path, sizeof(icon_path), "%s/icon.png", app->path);
                if (!app->icon) {
                    //printf("Loading icon for app: %s\n", app->name);
                    app->icon = XuiImgLoadFromFile(icon_path);

                    if (!app->icon) {
                        printf("Failed to load icon for app: %s\n", app->name);
                        continue;
                    }
                }

                int col = bi % GRID_W;
                int row = bi / GRID_W;
                unsigned int x = col * cell_w + GRID_MARGIN;
                unsigned int y = row * cell_h + GRID_MARGIN;

                //XuiCanvasDrawRect(state->root, x, y, icon_w, icon_h, XuiColor(255, 220, 220, 220), 1, 1);
                XuiCanvasDrawImg(state->root, x, y, icon_w, icon_h, XUI_BG_CENTER, app->icon);
                XuiWindow *btn = XuiCreateButton(state->root, x, y, icon_w, icon_h);
            

                if (!btn) {
                    printf("Failed to create button");
                    continue;
                }

                
                XuiButtonStat* btn_stat = malloc(sizeof(XuiButtonStat));
                if (!btn_stat) {
                    printf("malloc fail");
                    continue;
                }


                // I couldn't find a reason why img is not working, so I'm rendering an image bellow this and making this button transparent
                // If anyone has nerves to fix it, please do so

                *btn_stat = (XuiButtonStat){
                    .btn_bg = XuiColor(0, 255, 0, 255),
                    .img = *((XuiImg*) app->icon)
                };


                if (XuiButtonSetStat(btn, XUI_BTN_NORMAL, btn_stat) != 0) {
                    printf("Failed to set button stat");
                    free(btn_stat);
                    continue;
                }

                XuiButtonSetKey(btn, bi | GRID_KEYCODE);
                XuiShowWindow(btn, 1, 0);
            }
        }
    } else {
        XuiCanvasDrawText(state->root, (240 - XuiTextWidth(state->font, 5, "No Apps")) / 2, 140, 5, state->font, 0, XuiColor(255, 0, 0, 0), "No Apps");
    }
}

UIResult initui(ui_funcs* funcs, AppList *applist)
{
    ui_state state;
    int ticks = 0;
    size_t max_page = ceil((float) applist->count / (float) GRID_BUTTONS);
    char *xui_argv[] = {"ROTATE=90", "STATUSBAR=32"};
    
    state.funcs = funcs;
    state.page = 0;
    state.applist = applist;

    XuiOpen(sizeof(xui_argv) / sizeof(xui_argv[0]), xui_argv);

    state.root = XuiRootCanvas();
    state.font = XuiCreateFont("/usr/font/tm_font", 0, XUI_FONT_GREY);
    state.statusbar = XuiStatusbarCanvas();
    state.statusbar_battery = 0;

    draw_grid(&state);

    UIResult result = UI_RESULT_NONE;
    while (result == UI_RESULT_NONE) {
        if (XuiHasKey()) {
            int key = XuiGetKey();
            if (key < 0) {
                printf("Got error getting key: %d\n", key);
                result = UI_RESULT_RELAUNCH;
            } else if (key == (XUI_KEYENTER)) {
                if (1 < max_page) {
                    if (state.page + 1 >= max_page) {
                        state.page = 0;
                    } else {
                        state.page += 1;
                    }
                    draw_grid(&state);
                }
                printf("page: %d/%d\n", state.page, max_page);
            } else if (key == (XUI_KEYCLEAR)) {
                destroyui(&state);
                int supply = funcs->OsCheckPowerSupply();
                printf("Result: suspend - power supply: %d\n", supply);
                if (supply == POWER_BATTERY) {
                    //Deep sleep
                    funcs->OsSysSleepEx(2);
                } else {
                    //Plugged in so just shut the screen, or will wake up again
                    funcs->OsSysSleepEx(1);
                }
            } else if (key == XUI_KEYCANCEL) {
            	printf("Launching tm\n");
                destroyui(&state);
                clear_framebuffer();
                LoadApp("/usr/bin", "tm");
                clear_framebuffer(); //Clear after app ran
                result = UI_RESULT_RELAUNCH;
            } else if (key == (XUI_KEYENTER + XUI_KEYCANCEL)) {
                destroyui(&state);
                clear_framebuffer();
                result = UI_RESULT_EXIT;
            } else if ((key & GRID_KEYCODE) == GRID_KEYCODE) {
                //Virtual keycodes from UI
                result = launch_app(&state, key & 0xFF);
            } else if (key >= XUI_KEY1 && key <= XUI_KEY9) {
                //1 to 9 physical keys
                result = launch_app(&state, key - XUI_KEY1);
            } else if (key == XUI_KEYFUNC) {
                result = launch_app(&state, 9);
            } else if (key == XUI_KEY0) {
                result = launch_app(&state, 10);
            } else if (key == XUI_KEYALPHA) {
                result = launch_app(&state, 11);
            } else {
                printf("Unknown key: %d\n", key);
            }
            continue;
        } else {
            if (ticks <= 0) {
                ticks = 5;
                draw_statusbar(&state, false);
            }
            ticks--;
            funcs->OsSleep(100);
        }
    }

    return result;
}
