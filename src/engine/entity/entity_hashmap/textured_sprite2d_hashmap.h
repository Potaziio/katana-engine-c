#ifndef TEXTURED_SPRITE2D_HASHMAP_H
#define TEXTURED_SPRITE2D_HASHMAP_H

#include "../entity.h"

#define GLFW_INCLUDE_NONE
#include "../../../../include/GLFW/include/GLFW/glfw3.h"
#include "../../../../include/GL/include/GL/glew.h"

#include <string.h>

struct textured_sprite2d_hashmap
{
    entity* key;
    struct textured_sprite2d* value;
    size_t size;
};

void textured_sprite2d_hashmap_create(struct textured_sprite2d_hashmap* map);
void textured_sprite2d_hashmap_add(struct textured_sprite2d_hashmap* map, entity key);
void textured_sprite2d_hashmap_pop(struct textured_sprite2d_hashmap* map, entity key);
void textured_sprite2d_hashmap_free(struct textured_sprite2d_hashmap* map);

struct textured_sprite2d* textured_sprite2d_hashmap_get(struct textured_sprite2d_hashmap* map, entity key);


#endif

