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
#define BUFSIZE 48000 
#define SAMPLE_RATE 48000

SDL_AudioDeviceID device_id;
int16_t samples[BUFSIZE];

typedef struct _UserData {
    uint16_t frequency;
    float amplitude;
} UserData;

void callback(void *userdata, uint8_t *stream, int streamLength)
{
    static uint8_t pos = 0;
    static uint32_t length = 0;
    SDL_LockAudioDevice(device_id);
    UserData *data = (UserData *)userdata;
    uint16_t max_amp = pow(2, 15) - 1;

    for (int i = pos; i < BUFSIZE; ++i) {
        float t = i * 1 / (float)SAMPLE_RATE;
        double a = cos(2 * M_PI * t * data->frequency) * max_amp * data->amplitude;
        samples[i] = a;
    }

    memcpy(stream, &samples, streamLength);
    pos += length;
    length -= length;
    SDL_UnlockAudioDevice(device_id);
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_AUDIO);

    UserData userdata;
    if (argc == 3) {
        sscanf(argv[1], "%hu", &userdata.frequency);
        sscanf(argv[2], "%f", &userdata.amplitude);

    }else {
        userdata.frequency = 400;
        userdata.amplitude = 0.5f;
    }

    SDL_AudioSpec wavSpec = {
        .format = AUDIO_S16,
        .freq = SAMPLE_RATE,
        .channels = 1,
        .size = SAMPLE_RATE,
        .samples = BUFSIZE,
        .userdata = &userdata,
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
    while(1);
    SDL_CloseAudioDevice(device_id);
    SDL_FreeWAV(wavStart);
    SDL_Quit();
}
