#ifndef CHUNK_H
#define CHUNK_H

#include "../../engine/core/engine.h"

#define CHUNK_WIDTH 8
#define CHUNK_HEIGHT 6
#define CHUNK_TILE_SCALE 32

extern struct texture map_atlas;

enum CHUNK_TILE_TYPE
{
    CHUNK_TILE_TYPE_AIR = 0,
    CHUNK_TILE_TYPE_DIRT,
    CHUNK_TILE_TYPE_STONE,
    CHUNK_TILE_TYPE_SIZE,
};

struct chunk
{
    entity id;
    struct sprite2d_batch_complex* batch;
    struct vector2 origin;
    unsigned char enabled;
};

struct chunk* chunk_create(struct vector2 origin);
void chunk_render(struct chunk* chunk);

#endif

