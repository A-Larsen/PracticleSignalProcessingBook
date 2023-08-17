#include <SDL_audio.h>
#include <stdio.h>
#include <SDL.h>
#include <memory.h>
#include <math.h>
#include <stdlib.h>

#define SDL_main main
#include <stdint.h>

#define ERROR() fprintf(stderr, "%s\n", SDL_GetError())
#define BUFSIZE 48000 
#define SAMPLE_RATE 48000

SDL_AudioDeviceID device_id;
uint8_t samples[BUFSIZE];

void callback(void *userdata, uint8_t *stream, int streamLength)
{
    static uint8_t pos = 0;
    static uint32_t length = 0;
    SDL_LockAudioDevice(device_id);
    for (int i = pos; i < BUFSIZE; ++i) {
        double r = (double)rand() / (double)RAND_MAX;
        // only doing * .01 to adjust the gain.
        samples[i] = floor(r * (256.0f * 0.01f));
    }
    memcpy(stream, &samples, BUFSIZE);
    SDL_UnlockAudioDevice(device_id);
    pos += length;
    length -= length;
}

int main(void)
{
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec wavSpec = {
        .format = AUDIO_S16,
        .freq = SAMPLE_RATE,
        .channels = 1,
        .size = SAMPLE_RATE,
        .samples = BUFSIZE,
        .userdata = NULL,
        .callback = callback
    };

    uint8_t * wavStart;
    uint32_t wavLength;


    device_id = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 
                                    SDL_AUDIO_ALLOW_FREQUENCY_CHANGE |
                                    SDL_AUDIO_ALLOW_SAMPLES_CHANGE |
                                    SDL_AUDIO_ALLOW_CHANNELS_CHANGE);

    if(!device_id) {
        ERROR();
        return 1;
    }

    SDL_PauseAudioDevice(device_id, 0);
    SDL_Delay(4000);

    SDL_CloseAudioDevice(device_id);
    SDL_FreeWAV(wavStart);
    SDL_Quit();
}
