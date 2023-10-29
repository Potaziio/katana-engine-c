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

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct textured_sprite2d*)realloc(map->value, sizeof(struct textured_sprite2d) * (map->size + 1));
}

void textured_sprite2d_hashmap_pop(struct textured_sprite2d_hashmap* map, entity key)
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

    /* texture_free(&map->value[index].texture); */
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

    map->key = (entity*)realloc(map->key, (last_element) * sizeof(entity));
    map->value = (struct textured_sprite2d*)realloc(map->value, (last_element) * sizeof(struct textured_sprite2d));

    map->size--;
}

void textured_sprite2d_hashmap_free(struct textured_sprite2d_hashmap* map)
{
    logger_log_string(WARNING, "Freeing textured_sprite2D map memory!!\n");

    for (int i = 0; i < map->size; i++)
    {
        texture_free(&map->value[i].texture);
        glDeleteBuffers(1, &map->value[i].vbo);
        glDeleteBuffers(1, &map->value[i].ebo);
        glDeleteVertexArrays(1, &map->value[i].vao);
    }

    free(map->key);
    free(map->value);
}

struct textured_sprite2d* textured_sprite2d_hashmap_get(struct textured_sprite2d_hashmap* map, entity key)
{
    for (int i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    return 0;
}

