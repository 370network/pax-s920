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
 * AI code (c) 2017 Moshe Piekarski
 *
 * ToDo convert all score to pinky location
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

#include "arcade.h"
#include "pacbox.h"
#include "pacbox_lcd.h"
#include "wsg3.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <linux/input.h>
#include <linux/soundcard.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define TIME_AFTER(a,b)         ((long)(b) - (long)(a) < 0)
#define TIME_BEFORE(a,b)        TIME_AFTER(b,a)

static unsigned short *lcd_fb;
static int button_fd = -1;
static int snd_fd = -1;

struct pacman_settings {
    int difficulty;
    int numlives;
    int bonus;
    int ghostnames;
    int showfps;
    int sound;
    int ai;
};

#define ioset(field, argument)\
{\
    int arg = argument;\
    if(ioctl(snd_fd, field, &arg)<0)\
        perror(#field "ioctl failed");\
    else if(arg!=argument)\
        perror(#argument "was not set in ioctl");\
}

unsigned long get_tick() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static struct pacman_settings settings;

static bool loadFile( const char * name, unsigned char * buf, int len )
{
    char filename[PATH_MAX];

    snprintf(filename,sizeof(filename), "pacman/%s",name);

    int fd = open( filename, O_RDONLY);

    if( fd < 0 ) {
        return false;
    }

    int n = read( fd, buf, len);

    close( fd );

    if( n != len ) {
        return false;
    }

    return true;
}

static bool loadROMS( void )
{
    bool romsLoaded = false;

    romsLoaded = loadFile( "pacman.6e", ram_,           0x1000) &&
                 loadFile( "pacman.6f", ram_+0x1000,    0x1000) &&
                 loadFile( "pacman.6h", ram_+0x2000,    0x1000) &&
                 loadFile( "pacman.6j", ram_+0x3000,    0x1000) &&
                 loadFile( "pacman.5e", charset_rom_,   0x1000) &&
                 loadFile( "pacman.5f", spriteset_rom_, 0x1000);

    if( romsLoaded ) {
        decodeROMs();
        reset_PacmanMachine();
    }

    return romsLoaded;
}

/* A buffer to render Pacman's 244x288 screen into */
static unsigned char video_buffer[ScreenWidth*ScreenHeight] __attribute__ ((aligned (16)));

static long start_time;
unsigned long slp_start_time, slp_end_time, slp_elapsed_time, slp_sleep_time;
static long video_frames = 0;

static int dipDifficulty[] = { DipDifficulty_Normal, DipDifficulty_Hard };
static int dipLives[] = { DipLives_1, DipLives_2, DipLives_3, DipLives_5 };
static int dipBonus[] = { DipBonus_10000, DipBonus_15000, DipBonus_20000, 
                          DipBonus_None };
static int dipGhostNames[] = { DipGhostNames_Normal, DipGhostNames_Alternate };

static int settings_to_dip(struct pacman_settings settings)
{
    return ( DipPlay_OneCoinOneGame | 
             DipCabinet_Upright | 
             DipMode_Play |
             DipRackAdvance_Off |

             dipDifficulty[settings.difficulty] |
             dipLives[settings.numlives] |
             dipBonus[settings.bonus] |
             dipGhostNames[settings.ghostnames]
           );
}

/* Sound is emulated in ISR context, so not much is done per sound frame */
#define NBSAMPLES    1448
static int16_t raw_buf[NBSAMPLES];

/*
    Audio callback
 */
static void get_more(const void **start, size_t *size)
{

    /* Emulate the audio for the current register settings */
    playSound(raw_buf, NBSAMPLES);

    //write(snd_fd, sound_buf, NBSAMPLES*sizeof(sound_buf[0]));
    write(snd_fd, raw_buf, sizeof(raw_buf));

    //*start = sound_buf;
    //*size = NBSAMPLES*sizeof(sound_buf[0]);
}

/* use buttons for joystick */
void joystick(void)
{
    struct input_event ev0;
    int rd;
    rd = read (button_fd, &ev0, sizeof (struct input_event));
    if (rd < sizeof (struct input_event))
        return;

    if (ev0.type != 1)
        return;

    if (ev0.value != 0 && ev0.value != 1)
        return;

    switch (ev0.code){
        case 223:
            exit (1);
            break;
        case 3:
            if (ev0.value == 1)
                setDeviceMode(Joy1_Up, DeviceOn);
            else
                setDeviceMode(Joy1_Up, DeviceOff);
            break;
        case 5:
            if (ev0.value == 1)
                setDeviceMode(Joy1_Left, DeviceOn);
            else
                setDeviceMode(Joy1_Left, DeviceOff);
            break;
        case 7:
            if (ev0.value == 1)
                setDeviceMode(Joy1_Right, DeviceOn);
            else
                setDeviceMode(Joy1_Right, DeviceOff);
            break;
        case 9:
            if (ev0.value == 1)
                setDeviceMode(Joy1_Down, DeviceOn);
            else
                setDeviceMode(Joy1_Down, DeviceOff);
            break;
        case 14:
            if (ev0.value == 1)
                setDeviceMode(CoinSlot_1, DeviceOn);
            else
                setDeviceMode(CoinSlot_1, DeviceOff);
            break;
        case 28:
            if (ev0.value == 1)
                setDeviceMode(Key_OnePlayer, DeviceOn);
            else
                setDeviceMode(Key_OnePlayer, DeviceOff);
            break;
        default:
            break;
    }
}

/*
    Runs the game engine for one frame.
*/
//int test;
static int gameProc( void )
{
    //int fps;
    //int status;
    //long end_time;
    int frame_counter = 0;

    wsg3_set_sampling_rate(96000);

    while (1)
    {
        /* Run the machine for one frame (1/60th second) */
        run();

        //test++;
        //if (test > 500)
        //    return 0;

        frame_counter++;

        joystick();

        /* We only update the screen every third frame - Pacman's native 
           framerate is 60fps, so we are attempting to display 20fps */
        if (frame_counter == 60 / FPS) {

            frame_counter = 0;
            video_frames++;

            /* The following functions render the Pacman screen from the 
               contents of the video and color ram.  We first update the 
               background, and then draw the Sprites on top. 
            */
            renderBackground(video_buffer);
            renderSprites(video_buffer);


            blit_display(lcd_fb ,video_buffer);

            get_more(raw_buf, NULL);


            //fps = (video_frames*100*100) / (get_tick()-start_time);
            //printf("%d.%02d / %d fps\n",fps/10,fps%100,FPS);
        }
    }

    return 0;
}

int _init()
{
    printf("welcome to paxman | pacbox port by 370network\n");

    int fb_fd = open ("/dev/fb", O_RDWR);
    lcd_fb = mmap (0, 320 * 240 * 2, 3, 1, fb_fd, 0);

    int i, j;
    for (i = 0; i < 240; i++)
        for (j = 0; j < 320; j++)
            lcd_fb[j + i * 320] = 0;

    button_fd = open("/dev/keypad", O_RDWR);
    i = fcntl(button_fd, F_GETFL,0);
    fcntl(button_fd, F_SETFL, i | O_NONBLOCK);

    snd_fd = open("/dev/snd/dsp", O_WRONLY);
    ioset(SOUND_PCM_WRITE_BITS,     8);
    ioset(SOUND_PCM_WRITE_CHANNELS, 1);
    ioset(SOUND_PCM_WRITE_RATE,     48000);

    /* Set the default settings */
    settings.difficulty = 0; /* Normal */
    settings.numlives = 2;   /* 3 lives */
    settings.bonus = 0;      /* 10000 points */
    settings.ghostnames = 0; /* Normal names */
    settings.showfps = 0;    /* Do not show FPS */
    settings.sound = 1;      /* Sound off by default */
    settings.ai = 0;         /* AI off by default */

    /* Initialise the hardware */
    init_PacmanMachine(settings_to_dip(settings));

    /* Load the romset */
    if (loadROMS()) {
        start_time = get_tick()-1;

        gameProc();
    }
    

    return 0;
}
