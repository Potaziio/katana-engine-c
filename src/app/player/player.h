#ifndef PLAYER_H
#define PLAYER_H

#include "../../engine/core/engine.h"
#include "../map/game_map/game_map.h"

struct player
{
	entity id;
	struct transform* transform;
	struct textured_sprite2d* sprite;
	struct script* script;
	struct texture texture;
	struct aabb collider;
	struct vector2 velocity;
	struct vector2 delta_move;
	float speed;
	float jump_force;
	int grounded;
};

void player_create(struct player* player);
void player_move_and_collide(struct player* player, struct map_colliders* colliders, int jump_key);

#endif

