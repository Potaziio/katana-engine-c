#include "engine/core/engine.h"

#include "engine/utils/color.h"
#include "midas_curse/map/chunk.h"
#include "midas_curse/map/map.h"
#include "midas_curse/player/player.h"

void app_start(void);
void app_update(void);
void app_on_tick(void);
void app_end(void);
void app_preload(void);

int main(void)
{
	struct engine app_engine;

	engine_app_start_callback_func(&app_engine, app_start);
	engine_app_update_callback_func(&app_engine, app_update);
	engine_app_tick_update_callback_func(&app_engine, app_on_tick);
	engine_app_end_callback_func(&app_engine, app_end);
	engine_app_preload_callback_func(&app_engine, NULL);

	app_engine.engine_window = (struct window)
	{
		.width = 1440, 
			.height = 768,
			.title = "Katana Engine (dev 0.0)",
			.color = {RGBA_WHITE}
	};

	if (engine_init(&app_engine))
		logger_log_string(SUCCESS, "Engine init was sucessful.\n");

	engine_update(&app_engine);

	if (engine_end(&app_engine))
		logger_log_string(SUCCESS, "Engine exit was successful.\n");

	return 0;
}

struct ui_text* text;
struct map* map;
struct player player;
char dev_text[100];

void app_start(void)
{
	engine_camera.type = CAMERA_ORTHOGRAPHIC;
	global_engine->config = ENGINE_SILENCE_ENTITY_LOG | ENGINE_UPDATE_CAMERA_BOUNDS;

	// Load textures 
    texture_load(&player_texture, "../src/assets/player_test.png");
	texture_load(&map_atlas, "../src/assets/map_atlas.png");
	texture_load(&background_texture, "../src/assets/background.png");

	map = map_create(80, 80, vector2(0.0f, 600.0f));
	player = player_create(vector2(ENGINE_WINDOW_CENTER), map);

	text = ui_text_init(_engine_default_font, (struct rgba_color){RGBA_BLACK}, vector2(5.0f, 5.0f), 0.135f, "", 0);
}

void app_update(void)
{
	player_move(&player);
	ui_text_render(text);
}

void app_on_tick(void)
{
	sprintf(dev_text, "KATANA ENGINE DEV 1.0                          MIDAS CURSE\nCHUNKS: %d\nFPS: %d", map->chunk_num, global_engine->fps);

	ui_text_mod(text, dev_text, strlen(dev_text));
}

void app_end(void)
{
	map_end(map);
	texture_free(&map_atlas);
	texture_free(&background_texture);
	texture_free(&player_texture);
	ui_text_free(text);
}

