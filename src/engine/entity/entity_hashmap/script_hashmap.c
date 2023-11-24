#include "script_hashmap.h"

void script_hashmap_create(struct script_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct script*)malloc(sizeof(struct script));
    map->size = 1;
}

void script_hashmap_add(struct script_hashmap* map, entity key)
{
    size_t key_size = (map->size) * sizeof(entity);
    size_t value_size = sizeof(struct script) * (map->size);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct script*)realloc(map->value, value_size);

    map->key[map->size - 1] = key;
    map->size++;

    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of script map failed, add\n");
}

void script_hashmap_pop(struct script_hashmap* map, entity key)
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
            memcpy((map->value + current), (map->value + (current + 1)), sizeof(struct script)); 
            memcpy((map->key + current), (map->key + (current + 1)), sizeof(entity));   
        }
    }

    map->size--;

    size_t key_size = (map->size) * sizeof(entity);
    size_t value_size = sizeof(struct script) * (map->size);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct script*)realloc(map->value, value_size);
}

void script_hashmap_free(struct script_hashmap* map)
{
    logger_log_string(WARNING, "Freeing script map memory!!\n");
    free(map->key);
    free(map->value);
}

struct script* script_hashmap_get(struct script_hashmap* map, entity key)
{
    for (uint32_t i = 0; i < map->size - 1; i++)
        if (map->key[i] == key)
            return &map->value[i];

    logger_log_string(ERROR, "Script returning null, script_hashmap_get\n");

    return NULL;
}

