#include "flappy.h"
#include "player/player.h"
#include "wall/wall.h"
#include "../engine/ui/ui_menu/ui_menu.h"
#include <math.h>

player flappy_bird;

struct ui_menu main_menu;

enum MAIN_MENU_OPTIONS
{
	MAIN_MENU_PLAY,
	MAIN_MENU_QUIT,
	MAIN_MENU_OPTIONS_SIZE,
};

void texture_setup()
{
	texture_load(&player_texture, "../src/assets/flappy_bird.png");
	texture_load(&wall_texture, "../src/assets/wall.png");
}

void flappy_start(void)
{
	global_engine->config = ENGINE_SILENCE_ENTITY_LOG | ENGINE_UPDATE_CAMERA_BOUNDS;
	engine_camera.type = CAMERA_ORTHOGRAPHIC;

	texture_setup();

	flappy_bird = player_create();

	state.game_state = GAME_STATE_MAIN_MENU;
	state.player = &flappy_bird;

	walls_create(&state);

	ui_menu_allocate(&main_menu, MAIN_MENU_OPTIONS_SIZE);

	main_menu.active = UI_MENU_ACTIVE;
	main_menu.bottom_padding = (engine_camera.bounds.y * 0.5f) + 50;
	main_menu.horizontal_padding = 100.0f;
	main_menu.vertical_padding = 50.0f;

	ui_menu_begin_option(&main_menu, "PLAY", MAIN_MENU_PLAY, UI_MENU_ALIGNED);
	ui_menu_begin_option(&main_menu, "QUIT", MAIN_MENU_QUIT, UI_MENU_ALIGNED);
}


void flappy_update(void)
{
	switch (ui_menu_get_option_on_click(&main_menu))
	{
		case MAIN_MENU_PLAY:
			state.game_state = GAME_STATE_START;
			main_menu.active = UI_MENU_DISABLED;
			break;
		case MAIN_MENU_QUIT:
			window_close(&global_engine->engine_window);
			break;
	}

	if (ui_menu_get_option_on_click(&main_menu) == GAME_STATE_START)
	{
		state.game_state = GAME_STATE_START;
		main_menu.active = UI_MENU_DISABLED;
	}
		
	if (state.game_state == GAME_STATE_START)
		player_update(&flappy_bird);

	ui_menu_render(&main_menu);
	ui_text_render(flappy_bird.score_text);
}

void flappy_end(void)
{
	logger_log_string(SUCCESS, "Byeee!!!\n");
	texture_free(&player_texture);
	texture_free(&wall_texture);

	ui_menu_free(&main_menu);
}

