#include "debug_line_hashmap.h"

void debug_line_hashmap_create(struct debug_line_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct debug_line*)malloc(sizeof(struct debug_line));
    map->size = 0;
}

void debug_line_hashmap_add(struct debug_line_hashmap* map, entity key)
{
    map->key[map->size] = key;
    map->size++;

    size_t key_size = (map->size + 1) * sizeof(entity);
    size_t value_size = sizeof(struct script) * (map->size + 1);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct debug_line*)realloc(map->value, value_size);
}

void debug_line_hashmap_pop(struct debug_line_hashmap* map, entity key)
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

    glDeleteBuffers(1, &map->value[index].vbo);
    glDeleteVertexArrays(1, &map->value[index].vao);

    if (index != last_element)
    {
        for (uint32_t i = index + 1; i < map->size; i++)
        {
            map->value[i - 1] = map->value[i];
            map->key[i - 1] = map->key[i];
        }
    }

    map->size--;

    size_t key_size = (map->size + 1) * sizeof(entity);
    size_t value_size = sizeof(struct script) * (map->size + 1);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct debug_line*)realloc(map->value, value_size);
}

void debug_line_hashmap_free(struct debug_line_hashmap* map)
{
    logger_log_string(WARNING, "Freeing line map memory!!\n");

    for (uint32_t i = 0; i < map->size; i++)
    {
        glDeleteBuffers(1, &map->value[i].vbo);
        glDeleteVertexArrays(1, &map->value[i].vao);
    }

    free(map->key);
    free(map->value);
}

struct debug_line* debug_line_hashmap_get(struct debug_line_hashmap* map, entity key)
{
    for (uint32_t i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    return NULL;
}

