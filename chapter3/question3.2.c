#include <audio/audio.h>
#include <audio/mixer.h>
#include <audio/readWav.h>

#include <stdint.h>
#include <stdio.h>
#include <conio.h>

bool AUDIO_START = true;
Mixer mixer;

uint32_t data_len = 0;
int16_t *data;

typedef struct _UserData {
    WavData wavdata;
    int16_t *data;
    uint32_t data_len;
    WAVEFORMATEX format;
    uint32_t pos;
    float max_amp;
} UserData;


void mu_table(int8_t idx, uint16_t *quantized, bool *sign)
{
    *sign = (idx >> 7);
    uint16_t chord = (idx >> 4) & 0x7;
    uint16_t step = idx & 0xf;
    *quantized = ((2 * step) + 33) * pow(2, chord) - 33;
}
static void initStream(WavData *wavdata)
{
}

static void handleStream(int16_t *stream, WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
    if(userdata->pos > userdata->data_len) {
        return;
    }

    int16_t to_mu = (int16_t)((float)(userdata->data[userdata->pos]) * 0.25f);

    uint16_t prev = 0;

    int16_t out;

    for (int i = -128; i < 128; ++i) {
        uint16_t qu = 0;
        bool sign = 0;
        mu_table(i, &qu, &sign);
        if (qu > to_mu) {
            out = prev * 4;
            out *= sign ? -1 : 1;
            break;
        }
        prev = qu;
    }

    *stream = out * mixer.master_amp;
    userdata->pos += 1;

}
int main(int argc, char **argv)
{
    WavHeader wavheader;
    UserData userdata;
    readWav("question3.1.wav", &wavheader, &userdata.data, &userdata.data_len);
    userdata.format.wFormatTag = wavheader.wFormatTag;
    userdata.format.nChannels = wavheader.nChannels;
    userdata.format.nSamplesPerSec = wavheader.nSamplesPerSec;
    userdata.format.wBitsPerSample = wavheader.wBitsPerSample;
    userdata.format.cbSize = wavheader.cbSize;
    userdata.pos = 0;
    userdata.max_amp = pow(2, userdata.format.wBitsPerSample - 1) - 1;
    mixer.master_amp = .4;

    wav_init(&userdata.wavdata, initStream, handleStream, &userdata.format, &userdata);
    printf("sizeof %llu\n", sizeof(int8_t));
    userdata.wavdata.play = true;
    bool quit = false;
    while(!quit) {
        char key = _getch();
        switch (key) {
        case 'q': 
        {
            quit = true;
            break;
        }
            
        }
    };
    return 0;
}
