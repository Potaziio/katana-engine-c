#include "transform_hashmap.h"
#include <complex.h>
#include <string.h>

void transform_hashmap_create(struct transform_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct transform*)malloc(sizeof(struct transform));
    map->size = 1;
}

void transform_hashmap_add(struct transform_hashmap* map, entity key)
{
    size_t key_size = (map->size) * sizeof(entity);
    size_t value_size = sizeof(struct transform) * (map->size);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct transform*)realloc(map->value, value_size);

    map->key[map->size - 1] = key;

    map->size++;

    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of transform map failed, add\n");
}

void transform_hashmap_pop(struct transform_hashmap* map, entity key)
{
    int32_t entity_found = 0;
    int32_t index = 0;

    for (uint32_t i = 0; i < map->size - 1; i++)
    {
        if (map->key[i] == key)
        {
            entity_found = 1;
            index = i;
            break;
        }
    }

    if (!entity_found) return;

    int32_t last_element = map->size - 2;

    if (index != last_element)
    {
        for (int32_t current = index; current < last_element; current++)
        {
            memcpy((map->value + current), (map->value + (current + 1)), sizeof(struct transform)); 
            memcpy((map->key + current), (map->key + (current + 1)), sizeof(entity)); 
        }
    }

    map->size--;

    size_t key_size = map->size * sizeof(entity);
    size_t value_size = sizeof(struct transform) * map->size;

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct transform*)realloc(map->value, value_size);
}

void transform_hashmap_free(struct transform_hashmap* map)
{
    free(map->key);
    free(map->value);
}

struct transform* transform_hashmap_get(struct transform_hashmap* map, entity key)
{
    for (uint32_t i = 0; i < map->size - 1; i++)
        if (map->key[i] == key)
            return &map->value[i];

    logger_log_string(ERROR, "Returning null from transform_hashmap_get\n");

    return NULL;
}

