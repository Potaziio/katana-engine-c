#include "textured_sprite2d_hashmap.h"

void textured_sprite2d_hashmap_create(struct textured_sprite2d_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct textured_sprite2d*)malloc(sizeof(struct textured_sprite2d));
    map->size = 1;
}

void textured_sprite2d_hashmap_add(struct textured_sprite2d_hashmap* map, entity key)
{
    size_t key_size = (map->size) * sizeof(entity);
    size_t value_size = sizeof(struct textured_sprite2d) * (map->size);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct textured_sprite2d*)realloc(map->value, value_size);

    map->key[map->size - 1] = key;
    map->size++;

    if (map->key == NULL || map->value == NULL)
        logger_log_string(ERROR, "Reallocation of textured_sprite map failed, add\n");
}

void textured_sprite2d_hashmap_pop(struct textured_sprite2d_hashmap* map, entity key)
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
            memcpy((map->value + current), (map->value + (current + 1)), sizeof(struct textured_sprite2d)); 
            memcpy((map->key + current), (map->key + (current + 1)), sizeof(entity));        
        }
    }

    map->size--;

    size_t key_size = (map->size) * sizeof(entity);
    size_t value_size = (map->size) * sizeof(struct textured_sprite2d);

    map->key = (entity*)realloc(map->key, key_size);
    map->value = (struct textured_sprite2d*)realloc(map->value, value_size);
}

void textured_sprite2d_hashmap_free(struct textured_sprite2d_hashmap* map)
{
    logger_log_string(WARNING, "Freeing textured_sprite2D map memory!!\n");

    for (uint32_t i = 0; i < map->size - 1; i++)
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
    for (uint32_t i = 0; i < map->size - 1; i++)
        if (map->key[i] == key)
            return &map->value[i];
    
    logger_log_string(ERROR, "Returning null from textured_sprite2d_hashmap_get\n");

    return NULL;
}

