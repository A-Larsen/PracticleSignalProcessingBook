#ifndef _PLAY_WAV_H_
#define _PLAY_WAV_H_

#ifdef LUALIB
#include <lua.h>
#endif

#include "audio.h"
#include "readWav.h"
#include "mixer.h"

extern Mixer mixer;

typedef struct _WavPlayer {
    float max_amp;
    int16_t *data;
    uint32_t data_len;
    uint32_t pos;
    WAVEFORMATEX format;
    WavData wavdata;
} WavPlayer;


// lua state can be null
#ifdef LUALIB
 /* void WavPlayer_init(WavPlayer *userdata, const char *file_path, lua_State *L); */
 __declspec(dllexport) void WavPlayer_init(WavPlayer *userdata, const char *file_path, lua_State *L);
#else
/* void WavPlayer_init(WavPlayer *userdata, const char *file_path); */
__declspec(dllexport) void WavPlayer_init(WavPlayer *userdata, const char *file_path);
#endif

/* void WavPlayer_play(WavPlayer *userdata, bool play); */
__declspec(dllexport) void WavPlayer_play(WavPlayer *userdata, bool play);

#endif // _PLAY_WAV_H_
