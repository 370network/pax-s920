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
 * http://www.ascotti.org/
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

/* Platform-specific defines - used in both C and ASM files */

#ifndef _PACBOX_H
#define _PACBOX_H

#define PACMAN_UP       3
#define PACMAN_DOWN     9
#define PACMAN_LEFT     5
#define PACMAN_RIGHT    7
#define PACMAN_MENU     14
#define PACMAN_1UP      8
#define PACMAN_2UP      10
#define PACMAN_COIN     6

#define LCD_WIDTH  320
#define LCD_HEIGHT  240
#define LCD_SCALE  100
#define LCD_ROTATE 1
#define XOFS ((LCD_WIDTH-288)/2)
#define YOFS ((LCD_HEIGHT-224)/2)

#define FPS 60
#define TARGETFPS (1000000 / FPS)

/* 16kHz sounds pretty good - use it if available */
#define PREFERRED_SAMPLING_RATE SAMPR_16

#endif
