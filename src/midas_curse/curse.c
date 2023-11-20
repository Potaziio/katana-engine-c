#include "curse.h"

struct ui_text* text;
struct map* map;
struct player player;
char dev_text[100];

struct line line;

void game_start(void)
{
	global_engine->config = ENGINE_SILENCE_ENTITY_LOG | ENGINE_UPDATE_CAMERA_BOUNDS;
	engine_camera.type = CAMERA_ORTHOGRAPHIC;

	// Load textures 
    texture_load(&player_texture, "../src/assets/player_test.png");
	texture_load(&map_atlas, "../src/assets/map_atlas.png");
	texture_load(&background_texture, "../src/assets/background.png");

	map = map_create(20, 20, vector2(0.0f, 600.0f));
	player = player_create(vector2(ENGINE_WINDOW_CENTER), map);

	text = ui_text_init(_engine_default_font, (struct rgba_color){RGBA_BLACK}, vector2(10.0f, 10.0f), 0.135f, "HELLO WORLD", 11);
}

void game_update(void)
{
	player_move(&player);
	map_chunk_update(map);
	ui_text_render(text);
}

void game_on_tick(void)
{
	sprintf(dev_text, "KATANA ENGINE DEV 1.0\nCHUNKS: %d\nFPS: %d", map->chunk_num, global_engine->fps);
	ui_text_mod(text, dev_text, strlen(dev_text));
}

void game_end(void)
{
	map_end(map);
	texture_free(&map_atlas);
	texture_free(&background_texture);
	texture_free(&player_texture);
	ui_text_free(text);
}

