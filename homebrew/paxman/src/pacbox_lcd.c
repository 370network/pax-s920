/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Pacbox - a Pacman Emulator for Rockbox
 *
 * Based on PIE - Pacman Instructional Emulator
 *
 * Copyright (c) 1997-2003,2004 Alessandro Scotti
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#include "pacbox.h"
#include "pacbox_lcd.h"
#include "arcade.h"
#include "hardware.h"

void blit_display(unsigned short* lcd_framebuffer, unsigned char* vbuf)
{
    unsigned short* dst;
    unsigned short* next_dst;
    int x,y;

    next_dst=&lcd_framebuffer[YOFS*LCD_WIDTH+XOFS+ScreenHeight-1];
    for( y=ScreenHeight-1; y>=0; y-- ) {
        dst = (next_dst--);
        for( x=0; x<ScreenWidth; x++ ) {
            *dst = palette[*(vbuf++)];
            dst+=LCD_WIDTH;
        }
    }

}
