
#include "pax/sound.h"
#include "opusfile.h"
#include <future>
#include <iostream>
opus_int16 pcm[5760 * 16];
opus_int16 pcm_play[5760 * 16];
OggOpusFile *of = NULL;
std::atomic<bool> audio_ready(false);
void audio_set_gain(int gain)
{
    op_set_gain_offset(of, OP_HEADER_GAIN, gain);
}
void audio_thread()
{
    PAXSound snd;
    const size_t size = 1024; 
    char buffer[size]; 
	char path[256];
    if (getcwd(buffer, size) != NULL) {
		snprintf(path, sizeof(path), "%s/audio.opus", buffer);
        std::cout << "Audio path: " << path << std::endl;
    } 


    of = op_open_file(path, 0);
    if (of == NULL)
    {
        return;
    }
    audio_ready.store(true);
    int ret;
    while (1)
    {
        ret = op_read(of, pcm, 5760 * 8, 0);
        if (ret <= 0)
        {
            break;
        }
        for (int i = 0; i < ret; i++)
        {
            pcm_play[i * 2] = pcm[i];
            pcm_play[i * 2 + 1] = pcm[i];
        }
        void *ptr = pcm_play;
        while (ret > 0)
        {
            int written = snd.playSound(ptr, ret * 4);
            ret -= written / 2;
            ptr = (void *)((uint8_t *)ptr + written);
        }
    }
    op_free(of);
}