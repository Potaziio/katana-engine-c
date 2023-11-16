#ifndef SPRITE2D_BATCH_COMPLEX_H
#define SPRITE2D_BATCH_COMPLEX_H

#include "../entity.h"

#define GLFW_INCLUDE_NONE
#include "../../../../include/GLFW/include/GLFW/glfw3.h"
#include "../../../../include/GL/include/GL/glew.h"

struct sprite2d_batch_complex_hashmap
{
    entity* key;
    struct sprite2d_batch_complex* value;
    uint32_t size;
};

void sprite2d_batch_complex_hashmap_create(struct sprite2d_batch_complex_hashmap* map);
void sprite2d_batch_complex_hashmap_add(struct sprite2d_batch_complex_hashmap* map, entity key);
void sprite2d_batch_complex_hashmap_pop(struct sprite2d_batch_complex_hashmap* map, entity key);
void sprite2d_batch_complex_hashmap_free(struct sprite2d_batch_complex_hashmap* map);

struct sprite2d_batch_complex* sprite2d_batch_complex_hashmap_get(struct sprite2d_batch_complex_hashmap* map, entity key);

#endif

