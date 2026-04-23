#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <getopt.h>
#include <string>
#include <sys/time.h>
#include <vector>

extern "C" {
#include "types.h"
#include "hwdefs.h"
#include "emu.h"
#include "state.h"
#include "cpu.h"
#include "mmu.h"
#include "lcd.h"
#include "audio.h"
#include "disassembler.h"
#include "debugger.h"
#include "gui.h"
#include "fileio.h"
}

#include "pax/fb.h"
#include "pax/keypad.h"
#include "pax/touchscreen.h"
#include "pax/sound.h"

extern "C" {
#include "fbg_fbdev.h"
#include "fbgraphics.h"
}

#define GUI_WINDOW_TITLE "KoenGB"
#define GUI_ZOOM      4

#define AUDIO_ENABLE  1

PAXFramebuffer fb;
PAXKeypad kp;
PAXTouchscreen ts;
PAXSound snd;


uint8_t* audio_sndbuf;
int16_t audio_outbuf[AUDIO_SNDBUF_SIZE * AUDIO_CHANNELS];
int gui_audio_init(int sample_rate, int channels, size_t sndbuf_size,
        uint8_t *sndbuf) {
    (void)channels;
    (void)sndbuf_size;
    snd.setSamplerate(sample_rate);
    audio_sndbuf = sndbuf;
    return 0;
}


int gui_lcd_init(int width, int height, int zoom, const char *wintitle) {
    (void)width;
    (void)height;
    (void)zoom;
    (void)wintitle;
    return 0;
}


void gui_lcd_render_frame(char use_colors, uint16_t *pixbuf) {
    if (use_colors) {
        /* The colors stored in pixbuf are two byte each, 5 bits per rgb
         * component: -bbbbbgg gggrrrrr. We need to extract these, scale these
         * values from 0-1f to 0-ff and put them in RGBA format. For the scaling
         * we'd have to multiply by 0xff/0x1f, which is 8.23, approx 8, which is
         * a shift by 3. */
        for (int y_scr = 0; y_scr < 216; y_scr++) {
            for (int x_scr = 0;x_scr < 240;x_scr++) {
                int y = y_scr * 144 / 216;
                int x = x_scr * 160 / 240;
                int idx = x + y * GB_LCD_WIDTH;
                uint16_t rawcol = pixbuf[idx];
                uint32_t r = ((rawcol >>  0) & 0x1f) << 3;
                uint32_t g = ((rawcol >>  5) & 0x1f) << 3;
                uint32_t b = ((rawcol >> 10) & 0x1f) << 3;
                fb.pixel(x_scr, 268 - y_scr) = rgb16(r, g, b);
            }
        }
    } else {
        /* The colors stored in pixbuf already went through the palette
         * translation, but are still 2 bit monochrome. */
        uint32_t palette[] = { 0xffffffff, 0xaaaaaaaa, 0x66666666, 0x11111111 };
        for (int y_scr = 0; y_scr < 216; y_scr++) {
            for (int x_scr = 0;x_scr < 240;x_scr++) {
                int y = y_scr * 144 / 216;
                int x = x_scr * 160 / 240;
                int idx = x + y * GB_LCD_WIDTH;
                fb.pixel(x_scr, 268 - y_scr + 52) = palette[pixbuf[idx]];
            }
        }
    }
}

int gui_input_poll(struct player_input *input) {
    input->special_quit = 0;
    input->special_savestate = 0;
    input->special_dbgbreak = 0;

    //memset(input, 0, sizeof(struct player_input));
    KeyCode key = kp.getKey();
    switch (key) {
    case KEY_ALPHA: input->button_start = 1; break;
    case KEY_FUNC: input->button_select = 1; break;
    case KEY_9: input->button_b = 1; break;
    case KEY_3: input->button_a = 1; break;
    case KEY_8: input->button_down = 1; break;
    case KEY_2: input->button_up = 1; break;
    case KEY_4: input->button_left = 1; break;
    case KEY_6: input->button_right = 1; break;
    case KEY_ESC: input->special_quit = 1; break;
    default: break;
    }
    return 0;
}


int app_start(std::string rom_path) {
    struct gb_state gb_state;

    struct emu_args emu_args = {
        .rom_filename = NULL,
        .bios_filename = NULL,
        .state_filename = NULL,
        .save_filename = NULL,
        .break_at_start = 0,
        .print_disas = 0,
        .print_mmu = 0,
        .audio_enable = AUDIO_ENABLE,
    };

    emu_args.rom_filename = (char*)rom_path.c_str();

    if (emu_init(&gb_state, &emu_args)) {
        fprintf(stderr, "Initialization failed\n");
        return 1;
    }

    /* Initialize frontend-specific GUI */
    if (gui_lcd_init(GB_LCD_WIDTH, GB_LCD_HEIGHT, GUI_ZOOM, GUI_WINDOW_TITLE)) {
        fprintf(stderr, "Couldn't initialize GUI LCD\n");
        return 1;
    }
    if (emu_args.audio_enable) {
        if (gui_audio_init(AUDIO_SAMPLE_RATE, AUDIO_CHANNELS, AUDIO_SNDBUF_SIZE,
                    gb_state.emu_state->audio_sndbuf)) {
            fprintf(stderr, "Couldn't initialize GUI audio\n");
            return 1;
        }
    }

    printf("==========================\n");
    printf("=== Starting execution ===\n");
    printf("==========================\n\n");

    struct timeval starttime, endtime;
    gettimeofday(&starttime, NULL);


    while (!gb_state.emu_state->quit) {
        emu_step_frame(&gb_state);

        struct player_input input_state;
        memset(&input_state, 0, sizeof(struct player_input));
        gui_input_poll(&input_state);
        emu_process_inputs(&gb_state, &input_state);

        gui_lcd_render_frame(gb_state.gb_type == GB_TYPE_CGB,
                gb_state.emu_state->lcd_pixbuf);

        if (gb_state.emu_state->audio_enable) /* TODO */
            audio_update(&gb_state);
#if AUDIO_ENABLE == 1
        for(int i = 0;i < AUDIO_SNDBUF_SIZE * AUDIO_CHANNELS;i++) {
            audio_outbuf[i] = audio_sndbuf[i] * 16;
        }
        snd.playSound(audio_outbuf, sizeof(int16_t) * AUDIO_SNDBUF_SIZE * AUDIO_CHANNELS);
#endif
    }

    if (gb_state.emu_state->extram_dirty)
        emu_save(&gb_state, 1, gb_state.emu_state->save_filename_out);

    gettimeofday(&endtime, NULL);

    printf("\nEmulation ended at instr: ");
    disassemble(&gb_state);
    dbg_print_regs(&gb_state);

    int t_usec = endtime.tv_usec - starttime.tv_usec;
    int t_sec = endtime.tv_sec - starttime.tv_sec;
    double exectime = t_sec + (t_usec / 1000000.);

    double emulated_secs = gb_state.emu_state->time_seconds +
        gb_state.emu_state->time_cycles / 4194304.;

    printf("\nEmulated %f sec in %f sec WCT, %.0f%%.\n", emulated_secs, exectime,
            emulated_secs / exectime * 100);

    return 0;
}


__attribute__((constructor))
int app_start()
{

    struct _fbg *fbg = fbg_fbdevSetup((char*)"/dev/fb", 0); // you can also directly use fbg_fbdevInit(); for "/dev/fb0", last argument mean that will not use page flipping mechanism  for double buffering (it is actually slower on some devices!)

    struct _fbg_img *bb_font_img = fbg_loadImage(fbg, "bbmode1_8x8.png");
    struct _fbg_font *bbfont = fbg_createFont(fbg, bb_font_img, 8, 8, 33);

    // Set CWD to the app directory
    std::filesystem::current_path("roms");

    std::vector<std::string> files; // Name, isDir

    auto repopulate_files = [&files](){
        files.clear();
        auto path = std::filesystem::current_path(); //getting path
        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            if (std::filesystem::is_directory(entry)) {
                files.emplace_back(entry.path().filename().string() + "/");
            } else {
                files.emplace_back(entry.path().filename().string());
            }
        }

        if (path != "/") {
            files.emplace_back("../");
        }
        std::sort(files.begin(), files.end());
    };

    repopulate_files();
    
    std::size_t selected_idx = 0;
    KeyCode curKey = NONE;
    bool open_app = false;

    auto is_rom = [](const std::string& name) {
        return name.length() > 3 && name.substr(name.length() - 4) == ".gbc";
    };

    do {
        fbg_clear(fbg, 0); // can also be replaced by fbg_fill(fbg, 0, 0, 0);
        fbg_draw(fbg);
        fbg_write(fbg, (char*)"Select a rom to run", 0, 0);
        fbg_rect(fbg, 0, 8, 240, 8, 255, 255, 255);

        for(std::size_t i = 0;i < files.size();i++) {
            auto& name = files[i];
            if (selected_idx == i) {
                fbg_rect(fbg, 0, 16 + 8 * i, 240, 8, 255, 0, 0);

                if(open_app) {
                    if (name.back() == '/') {
                        std::filesystem::current_path(name);
                        repopulate_files();
                        selected_idx = 0;
                    } else {
                        // Clear the screen
                        for(int i = 0;i < 240;i++) {
                            for(int j = 0;j < 320;j++) {
                                fb.pixel(i, j) = rgb16(0, 0, 0);
                            }
                        }
                        app_start(name.c_str());
                        return 0;
                    }
                    open_app = false;
                }
            }
            if (is_rom(name)) {
                fbg_text(fbg, &fbg->current_font, (char*)name.c_str(), 0, 16 + 8 * i, 0, 255, 0);
            } else {
                fbg_text(fbg, &fbg->current_font, (char*)name.c_str(), 0, 16 + 8 * i, 255, 255, 255);
            }
        }

        fbg_flip(fbg);

        curKey = kp.getKey();
        switch (curKey) {
            case KEY_2: // UP
                selected_idx = std::max(selected_idx - 1, 0U);
                break;
            case KEY_8: // DOWN
                selected_idx = std::min(selected_idx + 1, files.size() - 1);
                break;
            case KEY_ENTER:
                open_app = true;
                break;
            default:
                break;
        }
    } while (curKey != KEY_ESC);

    fbg_freeImage(bb_font_img);
    fbg_freeFont(bbfont);
    fbg_close(fbg);

    return 0;
}