#include "sprite2d_hashmap.h"

void sprite2d_hashmap_create(struct sprite2d_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct sprite2d*)malloc(sizeof(struct sprite2d));
    map->size = 1;
}

void sprite2d_hashmap_add(struct sprite2d_hashmap* map, entity key)
{
    map->key = (entity*)realloc(map->key, (map->size) * sizeof(entity));
    map->value = (struct sprite2d*)realloc(map->value, sizeof(struct sprite2d) * (map->size));

    map->key[map->size - 1] = key;
    map->size++;


    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of sprite map failed, add\n");
}

void sprite2d_hashmap_pop(struct sprite2d_hashmap* map, entity key)
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

    glDeleteBuffers(1, &map->value[index].vbo);
    glDeleteBuffers(1, &map->value[index].ebo);
    glDeleteVertexArrays(1, &map->value[index].vao);

    if (index != last_element)
    {
        for (int32_t current = index; current < last_element; current++)
        {
            memcpy((map->value + current), (map->value + (current + 1)), sizeof(struct sprite2d)); 
            memcpy((map->key + current), (map->key + (current + 1)), sizeof(entity));         
        }
    }

    memset(map->value + (last_element), 0, sizeof(struct sprite2d));
    memset(map->key + (last_element), 0, sizeof(entity));

    map->size--;

    map->key = (entity*)realloc(map->key, (map->size) * sizeof(entity));
    map->value = (struct sprite2d*)realloc(map->value, (map->size) * sizeof(struct sprite2d));

    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of sprite map failed, add\n");
}

void sprite2d_hashmap_free(struct sprite2d_hashmap* map)
{
    logger_log_string(WARNING, "Freeing sprite2D map memory!!\n");

    for (uint32_t i = 0; i < map->size - 1; i++)
    {
        glDeleteBuffers(1, &map->value[i].vbo);
        glDeleteBuffers(1, &map->value[i].ebo);
        glDeleteVertexArrays(1, &map->value[i].vao);
    }

    free(map->key);
    free(map->value);
}

struct sprite2d* sprite2d_hashmap_get(struct sprite2d_hashmap* map, entity key)
{
    for (uint32_t i = 0; i < map->size - 1; i++)
        if (map->key[i] == key)
            return &map->value[i];

    logger_log_string(ERROR, "Returning null from sprite2d_hashmap_get\n");

    return NULL;
}

