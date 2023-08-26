#include <audio/playWav.h>

#include <stdio.h>
#include <conio.h>

bool AUDIO_START = true;
Mixer mixer;

int main(int argc, char **argv)
{
    mixer.master_amp = .4;
    WavPlayer player;
    WavPlayer_init(&player, "question3.1.wav");
    WavPlayer_play(&player, true);
    while(1);
    return 0;
}
