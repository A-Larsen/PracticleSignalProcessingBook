#include <SDL_audio.h>
#include <stdio.h>
#include <SDL.h>
#include <memory.h>
#include <math.h>
#include <stdlib.h>

#define SDL_main main
#include <stdint.h>

#define FILE_PATH ".\\question3.1.wav"
#define ERROR() fprintf(stderr, "%s\n", SDL_GetError())
#define BUFSIZE 2048

typedef struct AudioData {
    uint8_t *pos;
    uint32_t length;
} AudioData;
SDL_AudioDeviceID device_id;
uint8_t samples[BUFSIZE];

void callback(void *userdata, uint8_t *stream, int streamLength)
{
    SDL_LockAudioDevice(device_id);
    for (int i = 0; i < BUFSIZE; ++i) {
        double r = (double)rand() / (double)RAND_MAX;
        samples[i] = floor(r * 256.0f);
    }
    memcpy(stream, &samples, BUFSIZE);
    SDL_UnlockAudioDevice(device_id);
}

int main(void)
{
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec wavSpec = {
        .format = AUDIO_U8,
        .freq = 400,
        .channels = 1,
        .size = BUFSIZE,
        .samples = 2048,
        .callback = callback
    };

    uint8_t * wavStart;
    uint32_t wavLength;

    AudioData audio;
    audio.pos = wavStart;
    audio.length = wavLength;

    device_id = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 
                                    SDL_AUDIO_ALLOW_FREQUENCY_CHANGE |
                                    SDL_AUDIO_ALLOW_SAMPLES_CHANGE |
                                    SDL_AUDIO_ALLOW_CHANNELS_CHANGE);

    if(!device_id) {
        ERROR();
        return 1;
    }

    SDL_PauseAudioDevice(device_id, 0);
    while(audio.length > 0) SDL_Delay(100);

    SDL_CloseAudioDevice(device_id);
    SDL_FreeWAV(wavStart);
    SDL_Quit();
}
