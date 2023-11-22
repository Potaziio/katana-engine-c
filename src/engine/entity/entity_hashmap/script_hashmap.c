#include "script_hashmap.h"

void script_hashmap_create(struct script_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct script*)malloc(sizeof(struct script));
    map->size = 0;
}

void script_hashmap_add(struct script_hashmap* map, entity key)
{
    map->key[map->size] = key;
    map->size++;

    size_t key_size = (map->size + 1) * sizeof(entity);
    size_t value_size = sizeof(struct script) * (map->size + 1);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct script*)realloc(map->value, value_size);

    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of script map failed, add\n");
}

void script_hashmap_pop(struct script_hashmap* map, entity key)
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
            map->value = memmove((map->value + (i - 1)), (map->value + i), sizeof(struct script)); 
            map->key = memmove((map->key + (i - 1)), (map->key + i), sizeof(entity)); 
        }
    }

    if (map->value->data != NULL)
    {
        logger_log_string(WARNING, "Freeing script data\n");
        free(map->value->data);
    }

    map->size--;

    size_t key_size = (map->size + 1) * sizeof(entity);
    size_t value_size = sizeof(struct script) * (map->size + 1);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct script*)realloc(map->value, value_size);

    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of script map failed, pop\n");

}

void script_hashmap_free(struct script_hashmap* map)
{
    logger_log_string(WARNING, "Freeing script map memory!!\n");
    free(map->key);
    free(map->value);
}

struct script* script_hashmap_get(struct script_hashmap* map, entity key)
{
    for (uint32_t i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    logger_log_string(ERROR, "Script returning null, script_hashmap_get\n");

    return NULL;
}

