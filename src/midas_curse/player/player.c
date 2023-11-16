#include "player.h"

struct texture player_texture;

struct player player_create(struct vector2 position, struct map* map)
{
	struct player player;

	player.entity = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D);
	struct transform* t = ENTITY_GET_TRANSFORM(player.entity);
	struct textured_sprite2d* s = ENTITY_GET_TEXTURED_SPRITE2D(player.entity);
	t->position = vector2(position.x, position.y);
	t->scale = vector2(PLAYER_WIDTH, PLAYER_HEIGHT);
	s->texture = &player_texture;

	player.map = map;

	return player;
}

void player_move(struct player* player)
{
	struct transform* t = ENTITY_GET_TRANSFORM(player->entity);

	struct vector2 input = vector2(input_get_key(GLFW_KEY_A) ? -1.0f : input_get_key(GLFW_KEY_D) ? 1.0f : 0.0f,
								   input_get_key(GLFW_KEY_W) ? -1.0f : input_get_key(GLFW_KEY_S) ? 1.0f : 0.0f);
	vector2_normalize(&input);

	t->position.x += input.x * global_engine->delta_time * PLAYER_SPEED;
	t->position.y += input.y * global_engine->delta_time * PLAYER_SPEED;
}

