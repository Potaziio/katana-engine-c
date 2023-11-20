#include "textured_sprite2d_hashmap.h"

void textured_sprite2d_hashmap_create(struct textured_sprite2d_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct textured_sprite2d*)malloc(sizeof(struct textured_sprite2d));
    map->size = 0;
}

void textured_sprite2d_hashmap_add(struct textured_sprite2d_hashmap* map, entity key)
{
    map->key[map->size] = key;
    map->size++;

    size_t key_size = (map->size + 1) * sizeof(entity);
    size_t value_size = sizeof(struct textured_sprite2d) * (map->size + 1);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct textured_sprite2d*)realloc(map->value, value_size);
}

void textured_sprite2d_hashmap_pop(struct textured_sprite2d_hashmap* map, entity key)
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
    glDeleteBuffers(1, &map->value[index].ebo);
    glDeleteVertexArrays(1, &map->value[index].vao);

    if (index != last_element)
    {
        for (uint32_t i = index + 1; i < map->size; i++)
        {
            map->value[i - 1] = map->value[i];
            map->key[i - 1] = map->key[i];
        }
    }

    size_t key_size = (last_element + 1) * sizeof(entity);
    size_t value_size = (last_element + 1) * sizeof(struct textured_sprite2d);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct textured_sprite2d*)realloc(map->value, value_size);

    map->size--;
}

void textured_sprite2d_hashmap_free(struct textured_sprite2d_hashmap* map)
{
    logger_log_string(WARNING, "Freeing textured_sprite2D map memory!!\n");

    for (uint32_t i = 0; i < map->size; i++)
    {
        glDeleteBuffers(1, &map->value[i].vbo);
        glDeleteBuffers(1, &map->value[i].ebo);
        glDeleteVertexArrays(1, &map->value[i].vao);
    }

    free(map->key);
    free(map->value);
}

struct textured_sprite2d* textured_sprite2d_hashmap_get(struct textured_sprite2d_hashmap* map, entity key)
{
    for (uint32_t i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    return NULL;
}

