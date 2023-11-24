#ifndef MAP_H
#define MAP_H

#include "../../engine/core/engine.h"

#define COMBAT_MAP_COLS 25
#define COMBAT_MAP_ROWS 18 

typedef struct
{
    uint32_t tiles[COMBAT_MAP_ROWS * COMBAT_MAP_COLS];
    struct aabb collider[COMBAT_MAP_ROWS * COMBAT_MAP_COLS];  
    uint8_t collider_size;
    entity id;
} combat_map;

combat_map* combat_map_create(char* file);

#endif

