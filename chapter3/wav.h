#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <stdint.h>

#define _STR_WRITE(str, fp) fwrite(str, strlen(str), 1, fp)

typedef struct _WavSpec {
    uint16_t sample_rate;
    uint8_t bit_depth;
} WavSpec;

static void _data_write(int data, int size, FILE *fp)
{
    fwrite(&data, size, 1, fp);
}

int wav_write(char *file, WavSpec *wav, void *data, uint16_t length)
{
    // duration in seconds
    FILE *fp = NULL;

    // this will not work correctly if writing mode is not if binary mode "wb"
    if (fopen_s(&fp, file, "wb") != 0) {
        fprintf(stderr, "could not open file");
        return 1;
    }
    int *sample_array = (int *)data;

    // Header chunk
    _STR_WRITE("RIFF", fp);
    _STR_WRITE("----", fp);
    _STR_WRITE("WAVE", fp);
    _STR_WRITE("fmt ", fp);
    _data_write(16, 4, fp); // size
    _data_write(1, 2, fp); // compression code
    _data_write(1, 2, fp); // number of channels
    _data_write(wav->sample_rate, 4, fp); // SAMPLE_RATE
    _data_write((float)wav->sample_rate * (float)wav->bit_depth / 8.0f, 4, fp); // byte rate
    _data_write((float)wav->bit_depth / 8.0f, 2, fp); // block align
    _data_write(wav->bit_depth, 2, fp); // bit depth
    _STR_WRITE("data", fp);
    _STR_WRITE("----", fp);

    int preAudioPosition = ftell(fp);
    float max_amp = pow(2, wav->bit_depth -1) - 1;

    for (int i = 0; i < length; ++i) {
        int sample = sample_array[i];
        fwrite(&sample, sizeof(int), 1, fp);
    }

    // Header chunk
    int postAudioPosition = ftell(fp);
    fseek(fp, preAudioPosition - 4, 0);
    _data_write(postAudioPosition - preAudioPosition, 4 ,fp);
    fseek(fp, 4, SEEK_SET);
    _data_write(postAudioPosition - 8, 4, fp);
    fclose(fp);

    return 0;
}
