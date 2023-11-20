#include "wall.h"

game_state state;
struct texture wall_texture;

void create_wall(struct vector2 origin, float y_scale, void* game_state)
{
	entity w = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D | SCRIPT);

	struct transform* t = ENTITY_GET_TRANSFORM(w);
	struct textured_sprite2d* spr = ENTITY_GET_TEXTURED_SPRITE2D(w);
	struct script* sc = ENTITY_GET_SCRIPT(w);

	script_system_set_funcs(sc, NULL, wall_update_script);

	script_system_set_data(sc, game_state);

	t->position = origin;
	t->scale = vector2(WALL_WIDTH, y_scale);

	spr->texture = &wall_texture;
}

void walls_create(game_state* state)
{
	for (uint32_t i = 0; i < WALL_NUMBER; i++)
	{
		uint32_t bottom_y_scale = rand_int(WALL_MIN_Y_SCALE, WALL_MAX_Y_SCALE);
		uint32_t top_y_scale = rand_int(WALL_MIN_Y_SCALE, WALL_MAX_Y_SCALE);

		struct vector2 bottom = vector2((engine_camera.bounds.x * 0.5f) + WALL_CENTER_PADDING + (WALL_DISTANCE * i), engine_camera.bounds.y - bottom_y_scale * 0.5f);
		struct vector2 top = vector2(bottom.x, top_y_scale * 0.5f);

		create_wall(bottom, bottom_y_scale, (void*)state);
		create_wall(top, top_y_scale, (void*)state);
	}
}

void wall_update_script(entity e)
{
	struct transform* t = ENTITY_GET_TRANSFORM(e);
	struct script* sc = ENTITY_GET_SCRIPT(e);

	game_state* state = (game_state*)sc->data;

	if (state->game_state == GAME_STATE_MAIN_MENU || state->player->dead) return;

	t->position.x -= WALL_SPEED * global_engine->delta_time;

	struct aabb collider;

	collider.position = vector2(t->position.x - t->scale.x * 0.5f, t->position.y - t->scale.y * 0.5f);
	collider.scale = t->scale;

	if (physics_aabb_in_aabb(collider, state->player->collider))
	{
		player_die(state->player);
	}

	if (t->position.x + t->scale.x * 0.5f < 0.0f)
	{
		uint32_t new_scale = rand_int(WALL_MIN_Y_SCALE, WALL_MAX_Y_SCALE);

		t->scale.y = new_scale; 

		if (t->position.y < engine_camera.bounds.y * 0.5f) // Is a top wall
			t->position.y = new_scale * 0.5f;
		else 
			t->position.y = engine_camera.bounds.y - new_scale * 0.5f;

		t->position.x = engine_camera.bounds.x;
	}
}
