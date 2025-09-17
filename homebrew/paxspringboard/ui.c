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
const int GRID_MARGIN = 8;
const int GRID_BUTTONS = GRID_W * GRID_H;
const uint16_t GRID_KEYCODE = 0xE000;

typedef struct {
    AppList *applist;
    XuiWindow *root;
    XuiFont *font;
    XuiWindow *statusbar;
    XuiWindow *grid;
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
    if (state->grid) {
        XuiDestroyWindow(state->grid);
        state->grid = 0;
    }
    //Free up icons
    for (int i = 0; i < state->applist->count; i++) {
        XuiImgFree(state->applist->apps[i].icon);
        state->applist->apps[i].icon = 0;
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
        LoadApp(state->applist->apps[app_index].app);
        clear_framebuffer(); //Clear after app ran
        return UI_RESULT_RELAUNCH;
    } else {
        return UI_RESULT_NONE;
    }
}

void draw_grid(ui_state *state) {
    AppList *applist = state->applist;
    int cell_w = ceil(240.0 / (float) GRID_W);
    int cell_h = ceil(280.0 / (float) GRID_H);
    int icon_w = cell_w - GRID_MARGIN * 2;
    int icon_h = cell_h - GRID_MARGIN * 2;
    
    if (state->grid) {
        XuiDestroyWindow(state->grid);
    }
    state->grid = XuiCreateCanvas(state->root, 0, 0, 240, 280);
    XuiCanvasSetBackground(state->grid, XUI_BG_NORMAL, NULL, XuiColor(255, 255, 255, 255));
    XuiShowWindow(state->grid, 1, 0);

    if (applist && applist->count > 0) {
        for (int bi = 0; bi < GRID_BUTTONS; bi++) {
            int i = state->page * GRID_BUTTONS + bi;
            if (i >= applist->count) {
                break;
            }
            AppMetadata *app = &applist->apps[i];
            if (app && app->name) {
                printf("Adding app %d: %s\n", i, applist->apps[i].name);

                char icon_path[256];
                snprintf(icon_path, sizeof(icon_path), "/data/app/MAINAPP/apps/%s/icon.png", app->app);
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

                //XuiCanvasDrawRect(state->grid, x, y, icon_w, icon_h, XuiColor(255, 220, 220, 220), 1, 1);
                XuiCanvasDrawImg(state->grid, x, y, icon_w, icon_h, XUI_BG_CENTER, app->icon);
                XuiWindow *btn = XuiCreateButton(state->grid, x, y, icon_w, icon_h);
            

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
        XuiCanvasDrawText(state->grid, (240 - XuiTextWidth(state->font, 5, "No Apps")) / 2, 140, 5, state->font, 0, XuiColor(255, 0, 0, 0), "No Apps");
    }
}

UIResult initui(AppList *applist)
{
    ui_state state;
    size_t max_page = ceil((float) applist->count / (float) GRID_BUTTONS);
    char *xui_argv[] = {"ROTATE=90", "STATUSBAR=32"};
    char *statusbar_text = "PaxBoard";
    
    state.page = 0;
    state.applist = applist;

    XuiOpen(sizeof(xui_argv) / sizeof(xui_argv[0]), xui_argv);

    state.root = XuiRootCanvas();
    state.font = XuiCreateFont("/usr/font/tm_font", 0, XUI_FONT_GREY);
    state.grid = 0;

    state.statusbar = XuiStatusbarCanvas();
    XuiCanvasSetBackground(state.statusbar, XUI_BG_NORMAL, 0, XuiColor(0, 128, 0, 0));
    XuiCanvasSetBackground(state.root, XUI_BG_NORMAL, 0, XuiColor(255, 255, 255, 255));
    
    // Battery and ethernet status in future? 
    // XuiSetStatusbarIcon(5, "/usr/share/tm/tm_icon_pageup.bmp");
	// XuiSetStatusbarIcon(7, "/usr/share/tm/tm_icon_pagedown.bmp");

    XuiCanvasDrawText(state.statusbar, 5, 15, 10, state.font, 0, XuiColor(255, 255, 255, 255), statusbar_text);

    XuiClearArea(state.root, 0, 0, 240, 288);

    draw_grid(&state);

    UIResult result = UI_RESULT_NONE;
    while (result == UI_RESULT_NONE) {
        int key = XuiGetKey();
        if (key < 0) {
            printf("Got error getting key: %d\n", key);
            result = UI_RESULT_RELAUNCH;
        } else if (key == (XUI_KEYENTER)) {
            if (state.page + 1 >= max_page) {
                state.page = 0;
            } else {
                state.page += 1;
            }
            printf("page: %d/%d\n", state.page, max_page);
            draw_grid(&state);
        } else if (key == (XUI_KEYCLEAR)) {
            destroyui(&state);
            result = UI_RESULT_SUSPEND;
        } else if (key == XUI_KEYCANCEL || key == (XUI_KEYCANCEL + XUI_KEYENTER)) {
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
    }

    return result;
}
