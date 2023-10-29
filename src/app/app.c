#include "app.h"
#include "player/player.h"
#include "map/map_editor/map_editor.h"

// TODO: Physics system?? 

int editing = 0;

struct game_map game_map;

struct player player;

void app_start(void)
{
	global_engine->config |= ENGINE_SILENCE_ENTITY_LOG;
	engine_camera.type = ORTHOGRAPHIC;

	if (editing) { 
		map_editor_set_layout();
		return;
	}

	game_map_create(&game_map);

	texture_load(&player.texture, "../src/app/assets/textures/player2.png");
	player_create(&player);
	player.transform->position = (struct vector3){rand_int(0, engine_camera.bounds.x), 300.0f, 0.0f};
}

void app_update(void)
{
	if (editing) 
	{ 
		map_editor_on_update();
		return;
	}

	struct vector2 input = {input_get_key(GLFW_KEY_A) ? -1 : input_get_key(GLFW_KEY_D) ? 1 : 0, 0.0f};

	player_move_and_collide(&player, &game_map.colliders, GLFW_KEY_SPACE);
	player.delta_move = (struct vector2){input.x * player.speed * global_engine->delta_time, 0.0};
}

void app_on_tick(void)
{
}

void app_end(void)
{
	if (editing) 
	{ 
		map_editor_serialize();
		return;
	}

	texture_free(&grass_texture);
	texture_free(&cobble_texture);
	texture_free(&player.texture);
	free(game_map.colliders.colliders);
}

