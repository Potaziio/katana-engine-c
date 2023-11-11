#include "sprite2d_hashmap.h"

void sprite2d_hashmap_create(struct sprite2d_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct sprite2d*)malloc(sizeof(struct sprite2d));
    map->size = 0;
}

void sprite2d_hashmap_add(struct sprite2d_hashmap* map, entity key)
{
    map->key[map->size] = key;
    map->size++;

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct sprite2d*)realloc(map->value, sizeof(struct sprite2d) * (map->size + 1));
}

void sprite2d_hashmap_pop(struct sprite2d_hashmap* map, entity key)
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
    glDeleteBuffers(1, &map->value[index].ebo);
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

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct sprite2d*)realloc(map->value, (map->size + 1) * sizeof(struct sprite2d));
}

void sprite2d_hashmap_free(struct sprite2d_hashmap* map)
{
    logger_log_string(WARNING, "Freeing sprite2D map memory!!\n");

    for (int i = 0; i < map->size; i++)
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
    for (int i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    return NULL;
}

