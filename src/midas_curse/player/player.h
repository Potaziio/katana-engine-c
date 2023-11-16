#ifndef PLAYER_H
#define PLAYER_H

#include "../../engine/core/engine.h"
#include "../map/map.h"

#define PLAYER_WIDTH 32 
#define PLAYER_HEIGHT 64

#define PLAYER_SPEED 6

extern struct texture player_texture;

struct player
{
    entity entity;
    struct map* map;
};

struct player player_create(struct vector2 position, struct map* map);
void player_move(struct player* player);

#endif

