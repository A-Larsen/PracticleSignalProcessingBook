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
    uint32_t max;
    uint32_t min;
    float range;
} UserData;

static void initStream(WavData *wavdata)
{
}


static void handleStream(int16_t *stream, WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
    if(userdata->pos > userdata->data_len) {
        return;
    }
    *stream = userdata->data[userdata->pos] * mixer.master_amp;
    userdata->pos += 1;
}

int main(int argc, char **argv)
{
    WavHeader wavheader;
    UserData userdata;
    memset(&userdata, 0, sizeof(UserData));
    readWav("question3.1.wav", &wavheader, &userdata.data, &userdata.data_len);
    userdata.format.wFormatTag = wavheader.wFormatTag;
    userdata.format.nChannels = wavheader.nChannels;
    userdata.format.nSamplesPerSec = wavheader.nSamplesPerSec;
    userdata.format.wBitsPerSample = wavheader.wBitsPerSample;
    userdata.format.cbSize = wavheader.cbSize;
    userdata.pos = 0;
    userdata.max_amp = pow(2, userdata.format.wBitsPerSample - 1) - 1;
    mixer.master_amp = .4;

    /* for (int i = 0; i < userdata.data_len; ++i) { */
    /*     if (userdata.data[i] > userdata.max) { */
    /*         userdata.max = userdata.data[i]; */
    /*     } */
    /*     if (userdata.data[i] < userdata.min) { */
    /*         userdata.min = userdata.data[i]; */
    /*     } */
    /* } */
    userdata.max = 32767;
    userdata.min = -32768;

    userdata.range = userdata.max - userdata.min;
    uint16_t quantization = 1 << 6;

    for (int i = 0; i < userdata.data_len; ++i) {
        int a = (float)userdata.data[i] * ((float)quantization / (float)userdata.range);
        userdata.data[i] = a * (userdata.range / quantization);
    }

    wav_init(&userdata.wavdata, initStream, handleStream, &userdata.format, &userdata);

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
