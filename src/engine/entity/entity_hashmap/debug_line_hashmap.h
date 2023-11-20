#ifndef DEBUG_LINE_HASHMAP_H
#define DEBUG_LINE_HASHMAP_H

#include "../entity.h"

#define GLFW_INCLUDE_NONE
#include "../../../../include/GLFW/include/GLFW/glfw3.h"
#include "../../../../include/GL/include/GL/glew.h"

struct debug_line_hashmap
{
    entity* key;
    struct debug_line* value;
    size_t size;
};

void debug_line_hashmap_create(struct debug_line_hashmap* map);
void debug_line_hashmap_add(struct debug_line_hashmap* map, entity key);
void debug_line_hashmap_pop(struct debug_line_hashmap* map, entity key);
void debug_line_hashmap_free(struct debug_line_hashmap* map);

struct debug_line* debug_line_hashmap_get(struct debug_line_hashmap* map, entity key);

#endif
