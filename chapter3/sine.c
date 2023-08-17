#include <SDL_audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <memory.h>
#include <math.h>
#include <stdlib.h>

#define SDL_main main
#include <stdint.h>

#define ERROR() fprintf(stderr, "%s\n", SDL_GetError())

// This is 4 seconds of audio 48000 * 4
#define BUFSIZE ((uint16_t)192000)
#define SAMPLE_RATE 48000

SDL_AudioDeviceID device_id;
int16_t samples[BUFSIZE];

void callback(void *userdata, uint8_t *stream, int streamLength)
{
    uint16_t *freq = (uint16_t *)userdata;
    SDL_LockAudioDevice(device_id);

    for (int i = 0; i < BUFSIZE; ++i) {
        float t = i * 1 / (float)SAMPLE_RATE;
        double a = cos(2 * M_PI * t * (*freq)) * 128;
        samples[i] = a;
    }

    memcpy(stream, &samples, BUFSIZE);
    SDL_UnlockAudioDevice(device_id);
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_AUDIO);

    uint16_t freq;
    if (argv[1]) {
        sscanf(argv[1], "%hu", &freq);

    }else {
        freq = 400;
    }

    SDL_AudioSpec wavSpec = {
        .format = AUDIO_S16,
        .freq = SAMPLE_RATE,
        .channels = 1,
        .size = SAMPLE_RATE,
        .samples = BUFSIZE,
        .userdata = &freq,
        .callback = callback
    };

    uint8_t * wavStart;
    uint32_t wavLength;


    device_id = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

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
