#include <stdio.h>
#include <memory.h>
#include <stdint.h>
#include <SDL_audio.h>
#include <SDL.h>

#define SDL_main main
#define FILE_PATH ".\\question3.1.wav"
#define ERROR() fprintf(stderr, "%s\n", SDL_GetError())

typedef struct AudioData {
    uint8_t *pos;
    uint32_t length;
    SDL_AudioSpec spec;
} AudioData;
SDL_AudioDeviceID device_id;

typedef struct _MuLaw {
    uint8_t sign;
    uint8_t chord;
    uint8_t step;
    uint16_t quantization_level;
} MuLaw;

void getMuLaw(uint8_t idx, MuLaw *mu)
{
    uint8_t num = idx - 128;
    mu->sign = num >> 7;
    mu->chord = (num >> 4) & 7;
    mu->step = num & 15;
    mu->quantization_level = ((2 * mu->step) + 33) *
                                 pow(2, mu->chord) - 33;
}

void callback(void *userdata, uint8_t *stream, int streamLength)
{
    SDL_LockAudioDevice(device_id);
    AudioData *audio = (AudioData *)userdata;


    if (audio->length == 0) return;
    uint32_t length = (uint32_t)streamLength;
    length = (length > audio->length ? audio->length : length);
    uint32_t max_amp = pow(2, 15) - 1;
    uint16_t *data = (uint16_t *)audio->pos;
    /* for (uint32_t i = 0; i < length; ++i) { */
    /*     data[i] *= .0f; */
    /* } */

    /* printf("%d\n", *audio->pos); */
    memcpy(stream, audio->pos, length);
    audio->pos += length;
    audio->length -= length;
    SDL_UnlockAudioDevice(device_id);
}


int main(void)
{
    SDL_Init(SDL_INIT_AUDIO);
    AudioData audio;
    memset(&audio, 0, sizeof(SDL_AudioSpec));
    memset(&audio.spec, 0, sizeof(SDL_AudioSpec));
    uint8_t * wavStart;
    uint32_t wavLength;
    SDL_AudioSpec *data = SDL_LoadWAV(FILE_PATH, &audio.spec, &wavStart,
                                      &wavLength);
    if (!(audio.spec.format & AUDIO_U16)) {
        return 1;
    }

    if(!data) {
        ERROR();
        return 1;
    }

    MuLaw mu;
    memset(&mu, 0, sizeof(MuLaw));

    /* int amp = 4; */
    /* for(uint32_t i = 0; i < wavLength; ++i) { */
    /*     printf("%u\n", wavStart[i]); */
    /*     /1* if (wavStart[i] - amp > 0) { *1/ */

    /*     /1*     wavStart[i] -= amp; *1/ */
    /*     /1* } *1/ */
    /* } */

    audio.pos = wavStart;
    audio.length = wavLength;
    audio.spec.callback = callback;
    audio.spec.userdata = &audio;
    device_id = SDL_OpenAudioDevice(NULL, 0, &audio.spec, NULL, 0);

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
