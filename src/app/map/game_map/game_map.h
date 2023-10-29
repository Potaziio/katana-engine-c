#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "../../../engine/core/engine.h"
#include "../map.h"

struct map_colliders
{
	struct aabb* colliders;
	int size;
};

struct game_map
{
    struct map_colliders colliders;    
	char* map_path;
	char* background_path;
};

void game_map_create(struct game_map* game_map);

#endif

