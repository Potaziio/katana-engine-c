#include "transform_hashmap.h"

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

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct transform*)realloc(map->value, sizeof(struct transform) * (map->size + 1));
}

void transform_hashmap_pop(struct transform_hashmap* map, entity key)
{
    int entity_found = 0;
    int index = 0;

    for (int i = 0; i < map->size; i++)
    {
        if (map->key[i] == key)
        {
            entity_found = 1;
            index = i;
        }
    }

    if (!entity_found) return;

    int last_element = map->size - 1;

    if (index != last_element)
    {
        for (int i = index + 1; i < map->size; i++)
        {
            map->value[i - 1] = map->value[i];
            map->key[i - 1] = map->key[i];
        }
    }

    map->size--;

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct transform*)realloc(map->value, (map->size + 1) * sizeof(struct transform));

}

void transform_hashmap_free(struct transform_hashmap* map)
{
    logger_log_string(WARNING, "Freeing transform map memory!!\n");
    free(map->key);
    free(map->value);
}

struct transform* transform_hashmap_get(struct transform_hashmap* map, entity key)
{
    for (int i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    return NULL;
}

