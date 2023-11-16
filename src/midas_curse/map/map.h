#ifndef MAP_H
#define MAP_H

#include "chunk.h"

struct map
{
    struct chunk** chunks;
    int chunk_num;
};

struct map* map_create(int expand_left, int expand_right, struct vector2 chunk_origin);
void map_end(struct map* map);

#endif

