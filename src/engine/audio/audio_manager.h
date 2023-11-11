#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../../include/miniaudio/miniaudio.h"
#include "../utils/io_utils.h"

#include "../utils/logger.h"

extern struct audio_manager* _engine_default_audio_manager;

struct audio_manager
{   
    ma_engine* engine;
    ma_engine_config config;
};

struct sound 
{
    char* file;
    ma_sound sound;
};

struct audio_manager* audio_manager_create(void);
struct sound* audio_manager_sound_load(struct audio_manager* manager, const char* file);
void audio_manager_play_sound(struct sound* sound);
void audio_manager_play_and_forget_sound(struct audio_manager* manager, const char* file);
void audio_manager_sound_uninit(struct sound* sound);

#endif

