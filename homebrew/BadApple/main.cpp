
#include "pax/fb.h"
#include "pax/keypad.h"
#include "pax/touchscreen.h"
#include "pax/sound.h"
#include "pax/printer.h"

#include <algorithm>
#include <fcntl.h>
#include <future>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <thread>
#include <utility>

#include <iostream>

void audio_set_gain(int gain);
extern std::atomic<bool> audio_ready;
void audio_thread();

__attribute__((constructor)) int app_start()
{
    PAXFramebuffer fb;
    PAXKeypad kp;
    PAXTouchscreen ts;
    // PAXSound snd;
    PAXPrinter pr;
    PAXPrinterCanvas canvas(288);

	const size_t size = 1024; 
    char buffer[size];   
	char path[256];     
    
    if (getcwd(buffer, size) != NULL) {
    	snprintf(path, sizeof(path), "%s/video.frames", buffer);
        std::cout << "Video path: " << path << std::endl;
    } 
    
    // Map the pixel data into memory
    int videofd = open(path, O_RDONLY);
    if (videofd == -1)
    {
        return 0;
    }
    // mmap the video file
    struct stat st;
    fstat(videofd, &st);
    uint8_t *video = (uint8_t *)mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, videofd, 0);
    if (video == (uint8_t *)-1)
    {
        return 0;
    }

    // Start the audio thread
	std::cout << "audio!";
    std::thread audio(audio_thread);
    audio.detach();
	
    // Time the frames to maintain 60fps
    int ptr = 0;
    int cur_pixels = 0;
    struct timeval start_ts;
    gettimeofday(&start_ts, 0);
    uint64_t target_us = 1000000 / 60;
    int cur_gain = -256*3*2;
    while (!audio_ready.load()) {
    	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
    audio_set_gain(cur_gain);
	std::cout << "audio load, get time, gain etc!";
    while (ptr < st.st_size)
    {
        // Frame is run-length encoded, msb is pixel value, rest of the byte is pixel count
        uint8_t cur_pixel_group = video[ptr];
        uint8_t cur_pixel = cur_pixel_group >> 7;
        uint8_t cur_pixel_cnt = cur_pixel_group & 0x7F;
        for (int i = 0; i < cur_pixel_cnt; i++)
        {
            // Screen is inverted, so we need to draw from the bottom up
            int x = cur_pixels % 240;
            int y = cur_pixels / 240;
            fb.drawPixel(x, 320 - 70 - y, cur_pixel * 0xFF, cur_pixel * 0xFF, cur_pixel * 0xFF);
            cur_pixels++;
        }
        ptr++;
        // Once we've drawn a full frame, handle events
        if (cur_pixels == 240 * 180)
        {

            KeyCode pressed = kp.getKey();

            if (pressed == KEY_ESC)
            {
                break;
            }
            if (pressed == KEY_1)
            {
                // Scale the image using nearest neighbor
                for (int j = 0; j < 384; j++)
                {
                    for (int i = 0; i < 288; i++)
                    {
                        canvas.setPixel(j, i, fb.pixel(j * 240 / 384, 320 - 70 - i * 180 / 288) == 0);
                    }
                }
                // Print the image asynchronously
                std::thread([&pr, &canvas]
                            { pr.print(canvas); })
                    .detach();
            }

            if (pressed == KEY_2)
            {
                cur_gain += 256;
                audio_set_gain(cur_gain);
            }
            if (pressed == KEY_8)
            {
                cur_gain -= 256;
                audio_set_gain(cur_gain);
            }

            auto ev = ts.getTouchEvent();
            auto [x, y, p] = ev;
            if (x != -1)
            {
                for (int i = 0; i < 320; i++)
                {
                    fb.drawPixel(x, i, 0xFF, 0, 0xFF);
                }
            }
            if (y != -1)
            {
                for (int i = 0; i < 240; i++)
                {
                    fb.drawPixel(i, y, 0, 0xFF, 0xFF);
                }
            }

            // Sleep to maintain 60fps
            struct timeval end_ts;
            gettimeofday(&end_ts, 0);
            uint64_t elapsed_us = (uint64_t)(end_ts.tv_sec - start_ts.tv_sec) * 1000000 + (end_ts.tv_usec - start_ts.tv_usec);
            if (elapsed_us < target_us)
            {
                struct timespec ts;
                ts.tv_sec = 0;
                ts.tv_nsec = (target_us - elapsed_us) * 1000;
                nanosleep(&ts, 0);
            }
            gettimeofday(&start_ts, 0);
            cur_pixels = 0;
        }
    }
    return 0;
}
