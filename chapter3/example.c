#include <SDL_audio.h>
#include <stdio.h>
#include <SDL.h>
#include <memory.h>

#define SDL_main main
#include <stdint.h>

#define FILE_PATH ".\\question3.1.wav"
#define ERROR() fprintf(stderr, "%s\n", SDL_GetError())

typedef struct AudioData {
    uint8_t *pos;
    uint32_t length;
} AudioData;
SDL_AudioDeviceID device_id;

void callback(void *userdata, uint8_t *stream, int streamLength)
{
    SDL_LockAudioDevice(device_id);
    AudioData *audio = (AudioData *)userdata;
    if (audio->length == 0) return;
    uint32_t length = (uint32_t)streamLength;
    length = (length > audio->length ? audio->length : length);

    memcpy(stream, audio->pos, length);

    audio->pos += length;
    audio->length -= length;
    SDL_UnlockAudioDevice(device_id);
}

int main(void)
{
    SDL_Init(SDL_INIT_AUDIO);
    SDL_AudioSpec wavSpec;
    memset(&wavSpec, 0, sizeof(SDL_AudioSpec));
    uint8_t * wavStart;
    uint32_t wavLength;

    SDL_AudioSpec *data = SDL_LoadWAV(FILE_PATH, &wavSpec, &wavStart, &wavLength);

    if(!data) {
        ERROR();
        return 1;
    }
    AudioData audio;
    audio.pos = wavStart;
    audio.length = wavLength;

    wavSpec.callback = callback;
    wavSpec.userdata = &audio;
    SDL_AudioSpec desired;
    desired.freq = 4000;
    desired.samples = 4096;
    device_id = SDL_OpenAudioDevice(NULL, 0, &wavSpec, &desired, 
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
