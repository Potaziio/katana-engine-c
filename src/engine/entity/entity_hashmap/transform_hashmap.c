#include "transform_hashmap.h"
#include <string.h>

void transform_hashmap_create(struct transform_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct transform*)malloc(sizeof(struct transform));
    map->size = 0;
}

void transform_hashmap_add(struct transform_hashmap* map, entity key)
{
    map->key[map->size] = key;

    map->size++;

    size_t key_size = (map->size + 1) * sizeof(entity);
    size_t value_size = sizeof(struct transform) * (map->size + 1);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct transform*)realloc(map->value, value_size);

    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of transform map failed, add\n");
}

void transform_hashmap_pop(struct transform_hashmap* map, entity key)
{
    int32_t entity_found = 0;
    int32_t index = 0;

    for (uint32_t i = 0; i < map->size; i++)
    {
        if (map->key[i] == key)
        {
            entity_found = 1;
            index = i;
        }
    }

    if (!entity_found) return;

    int32_t last_element = map->size - 1;

    if (index != last_element)
    {
        for (uint32_t i = index + 1; i < map->size; i++)
        {
            map->value = memmove((map->value + (i - 1)), (map->value + i), sizeof(struct transform)); 
            map->key = memmove((map->key + (i - 1)), (map->key + i), sizeof(entity)); 
        }
    }

    map->size--;

    size_t key_size = (map->size + 1) * sizeof(entity);
    size_t value_size = sizeof(struct transform) * (map->size + 1);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct transform*)realloc(map->value, value_size);

    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of transform map failed, pop\n");

}

void transform_hashmap_free(struct transform_hashmap* map)
{
    logger_log_string(WARNING, "Freeing transform map memory!!\n");
    free(map->key);
    free(map->value);
}

struct transform* transform_hashmap_get(struct transform_hashmap* map, entity key)
{
    for (uint32_t i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    logger_log_string(ERROR, "Returning null from transform_hashmap_get\n");

    return NULL;
}

