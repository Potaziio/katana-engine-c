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

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct debug_line*)realloc(map->value, sizeof(struct debug_line) * (map->size + 1));
}

void debug_line_hashmap_pop(struct debug_line_hashmap* map, entity key)
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

    glDeleteBuffers(1, &map->value[index].vbo);
    glDeleteVertexArrays(1, &map->value[index].vao);

    if (index != last_element)
    {
        for (int i = index + 1; i < map->size; i++)
        {
            map->value[i - 1] = map->value[i];
            map->key[i - 1] = map->key[i];
        }
    }

    map->size--;

    map->key = (entity*)realloc(map->key, map->size * sizeof(entity));
    map->value = (struct debug_line*)realloc(map->value, map->size * sizeof(struct debug_line));

}

void debug_line_hashmap_free(struct debug_line_hashmap* map)
{
    logger_log_string(WARNING, "Freeing line map memory!!\n");

    for (int i = 0; i < map->size; i++)
    {
        glDeleteBuffers(1, &map->value[i].vbo);
        glDeleteVertexArrays(1, &map->value[i].vao);
    }

    free(map->key);
    free(map->value);
}

struct debug_line* debug_line_hashmap_get(struct debug_line_hashmap* map, entity key)
{
    for (int i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    return 0;
}

