#ifndef TRANSFORM_HASHMAP_H
#define TRANSFORM_HASHMAP_H

#include "../entity.h"

struct transform_hashmap {
    entity* key;
    struct transform* value;
    size_t size;
};

void transform_hashmap_create(struct transform_hashmap* map);
void transform_hashmap_add(struct transform_hashmap* map, entity key);
void transform_hashmap_pop(struct transform_hashmap* map, entity key);
void transform_hashmap_free(struct transform_hashmap* map);

struct transform* transform_hashmap_get(struct transform_hashmap* map, entity key);

#endif
