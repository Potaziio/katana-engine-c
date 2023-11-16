#ifndef MAP_H
#define MAP_H

#include "chunk.h"

extern struct texture background_texture;

struct map
{
    struct chunk** chunks;
    uint32_t chunk_num;
    entity background;
};

struct map* map_create(int32_t expand_left, int32_t expand_right, struct vector2 chunk_origin);
void map_end(struct map* map);

#endif

