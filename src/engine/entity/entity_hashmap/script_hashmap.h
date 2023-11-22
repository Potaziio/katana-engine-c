#ifndef SCRIPT_HASHMAP_H
#define SCRIPT_HASHMAP_H

#include "../entity.h"
#include <string.h>

struct script_hashmap {
    entity* key;
    struct script* value;
    size_t size;
};

void script_hashmap_create(struct script_hashmap* map);
void script_hashmap_add(struct script_hashmap* map, entity key);
void script_hashmap_pop(struct script_hashmap* map, entity key);
void script_hashmap_free(struct script_hashmap* map);

struct script* script_hashmap_get(struct script_hashmap* map, entity key);

#endif

