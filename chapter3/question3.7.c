#include <audio/audio.h>
#include <audio/mixer.h>
#include <math.h>
#include <stdint.h>

#define M_PI ((double)3.14159265359)
#define TWOPI (M_PI + M_PI)

typedef struct _UserData {
    float amplitude;
    float max_amp;
    float amp_step;
    WavData wavdata;
    WAVEFORMATEX format;
    int16_t *stream;
} UserData;

bool AUDIO_START = true;
Mixer mixer;
static void initStream(WavData *wavdata)
{
}


static void handleStream(int16_t *stream, WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
    static uint32_t sample_number;
    float t = (float)sample_number * 1.0f / (float)userdata->format.nSamplesPerSec;
    float pos = 0;
    if (sample_number % 3 == 0) {
        t += 0.1f;
    }
    pos = 2 * M_PI * 1000 * t;
    /* float pos = 2 * M_PI * t / 8; */
    if(pos >= TWOPI)  sample_number = 0;
    *stream = cos(pos) * userdata->max_amp;
    sample_number++;
}

int main(void)
{
    UserData userdata;
    memset(&userdata, 0, sizeof(UserData));
    userdata.format.wFormatTag = WAVE_FORMAT_PCM;
    userdata.format.nChannels = 1;
    /* userdata.format.nSamplesPerSec = SAMPLING_RATE; */
    userdata.format.nSamplesPerSec = 8000;
    userdata.format.wBitsPerSample = 16;
    userdata.format.cbSize = 0;
    userdata.amplitude = 1.0f;
    userdata.amp_step = 0.01f;
    userdata.max_amp = (32767 * userdata.amplitude);
    mixer.master_amp = 0.4f;

    wav_init(&userdata.wavdata, initStream, 
            handleStream, &userdata.format, &userdata);

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
