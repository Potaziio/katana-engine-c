#ifndef SPRITE2D_HASHMAP_H
#define SPRITE2D_HASHMAP_H

#include "../entity.h"

#define GLFW_INCLUDE_NONE
#include "../../../../include/GLFW/include/GLFW/glfw3.h"
#include "../../../../include/GL/include/GL/glew.h"

struct sprite2d_hashmap
{
    entity* key;
    struct sprite2d* value;
    uint32_t size;
};

void sprite2d_hashmap_create(struct sprite2d_hashmap* map);
void sprite2d_hashmap_add(struct sprite2d_hashmap* map, entity key);
void sprite2d_hashmap_pop(struct sprite2d_hashmap* map, entity key);
void sprite2d_hashmap_free(struct sprite2d_hashmap* map);

struct sprite2d* sprite2d_hashmap_get(struct sprite2d_hashmap* map, entity key);

#endif

