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

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct script*)realloc(map->value, sizeof(struct script) * (map->size + 1));
}

void script_hashmap_pop(struct script_hashmap* map, entity key)
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

    if (map->value->data != NULL)
    {
        logger_log_string(WARNING, "Freeing script data\n");
        free(map->value->data);
    }

    map->size--;

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct script*)realloc(map->value, (map->size + 1) * sizeof(struct script));

}

void script_hashmap_free(struct script_hashmap* map)
{
    logger_log_string(WARNING, "Freeing script map memory!!\n");
    free(map->key);
    free(map->value);
}

struct script* script_hashmap_get(struct script_hashmap* map, entity key)
{
    for (int i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    return NULL;
}

