#ifndef UI_H
#define UI_H

#include <stdint.h>


#define FB_PATH "/dev/fb"
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define BPP 2 // Bytes per pixel for RGB565

// Typedef for color
typedef uint16_t color_t;

// Function prototypes
void draw_pixel(color_t *buffer, int x, int y, color_t color);
void draw_char(color_t *buffer, int x, int y, char c, color_t color);
void draw_text(color_t *buffer, int x, int y, const char *text, color_t color);
void clear_buffer(color_t *buffer, color_t color);

#endif // UI_H