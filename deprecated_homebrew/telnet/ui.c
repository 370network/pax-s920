#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/statvfs.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h> 


#include "font.h"
#include "ui.h"

typedef unsigned short color_t;

void draw_pixel(color_t *buffer, int x, int y, color_t color) {
    printf("draw_pixel: x=%d, y=%d, color=%u\n", x, y, color);
    if (y >= 0 && y < SCREEN_WIDTH && x >= 0 && x < SCREEN_HEIGHT) {
        buffer[(SCREEN_WIDTH * SCREEN_HEIGHT) - ((SCREEN_HEIGHT - x) * SCREEN_WIDTH + y)] = color;
    } else {
        printf("draw_pixel: Out of bounds\n");
    }
}

void draw_char(color_t *buffer, int x, int y, char c, color_t color) {
    printf("draw_char: x=%d, y=%d, char=%c, color=%u\n", x, y, c, color);
    if (c < 0 || c >= 128) {
        printf("draw_char: Character out of range\n");
        return; 
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((font8x8_basic[(int)c][j] >> i) & 1) {   
                printf("draw_char: Drawing pixel at x=%d, y=%d\n", x + i, y + j);
                draw_pixel(buffer, x + i, y + j, color); 
            }
        }
    }
}

void draw_text(color_t *buffer, int x, int y, const char *text, color_t color) {
    printf("draw_text: x=%d, y=%d, text=%s, color=%u\n", x, y, text, color);
    while (*text) {
        printf("draw_text: Drawing character '%c'\n", *text);
        draw_char(buffer, x, y, *text, color);
        x += 8;
        text++;
    }
}

void clear_buffer(color_t *buffer, color_t color) {
    printf("clear_buffer: Clearing buffer with color=%u\n", color);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        buffer[i] = color;
    }
}