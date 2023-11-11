#include "audio_manager.h"

struct audio_manager* _engine_default_audio_manager;

struct audio_manager* audio_manager_create(void)
{
    struct audio_manager* audio = (struct audio_manager*)malloc(sizeof(struct audio_manager));

    audio->engine = (ma_engine*)malloc(sizeof(ma_engine));

    if (audio->engine == NULL)
    {
        logger_log_string(ERROR, "Failed to allocate memory for miniaudio memory\n");
        return NULL;
    }

    audio->config = ma_engine_config_init();

    ma_result res = ma_engine_init(&audio->config, audio->engine);

    if (res != MA_SUCCESS)
    {
        logger_log_string(ERROR, "Mini engine audio creation failed\n");
        return NULL;
    }

    return audio;
}

struct sound* audio_manager_sound_load(struct audio_manager* manager, const char* file)
{
    struct sound* sound = (struct sound*)malloc(sizeof(struct sound));
    
    if (sound == NULL)
    {
        logger_log_string(ERROR, "Sound memory allocation failed\n");
        return NULL;
    }

    ma_result res = ma_sound_init_from_file(manager->engine, file, 0, NULL, NULL, &sound->sound);

    if (res != MA_SUCCESS)
    {
        logger_log_string(ERROR, "Error loading miniaudio sound error code: ");
        printf("%d\n", res);
        return NULL;
    }

    logger_log_string(LOG, "Loading audio from file: ");
    printf("%s\n", file);

    switch (res)
    {
        case MA_SUCCESS:
            logger_log_string(SUCCESS, "Success audio sound from file\n");
            break;
        case MA_INVALID_FILE:
            logger_log_string(ERROR, "Failed to find sound file: ");
            printf("%s\n", file);
            return NULL;
            break;
    };

    sound->file = (char*)malloc(strlen(file) * sizeof(char));
    sound->file = (char*)memcpy(sound->file, file, strlen(file) * sizeof(char));

    return sound;
}

void audio_manager_sound_uninit(struct sound* sound)
{
    ma_sound_uninit(&sound->sound);
}

void audio_manager_play_sound(struct sound* sound)
{
    ma_sound_start(&sound->sound);
}

void audio_manager_play_and_forget_sound(struct audio_manager* manager, const char* file)
{
    ma_engine_play_sound(manager->engine, file, NULL);
}   

