#ifndef CHUNK_H
#define CHUNK_H

#include "../../engine/core/engine.h"
#include "../../engine/render/line.h"

#define CHUNK_WIDTH 32
#define CHUNK_HEIGHT 12
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
    uint8_t enabled;
};

struct chunk* chunk_create(struct vector2 origin);
void chunk_update(struct chunk* chunk);

#endif

