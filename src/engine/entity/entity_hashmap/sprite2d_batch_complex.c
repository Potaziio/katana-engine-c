#include "sprite2d_batch_complex.h"

void sprite2d_batch_complex_hashmap_create(struct sprite2d_batch_complex_hashmap* map)
{
    map->key = (entity*)malloc(sizeof(entity));
    map->value = (struct sprite2d_batch_complex*)malloc(sizeof(struct sprite2d_batch_complex));
    map->size = 0;
}

void sprite2d_batch_complex_hashmap_add(struct sprite2d_batch_complex_hashmap* map, entity key)
{
    map->key[map->size] = key;
    map->size++;

    map->key = (entity*)realloc(map->key, (map->size + 1) * sizeof(entity));
    map->value = (struct sprite2d_batch_complex*)realloc(map->value, sizeof(struct sprite2d_batch_complex) * (map->size + 1));

}

void sprite2d_batch_complex_hashmap_pop(struct sprite2d_batch_complex_hashmap* map, entity key)
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

    free(map->value[index].indices);
    free(map->value[index].vertices);

    if (index != last_element)
    {
        for (uint32_t i = index + 1; i < map->size; i++)
        {
            map->value[i - 1] = map->value[i];
            map->key[i - 1] = map->key[i];
        }
    }

    map->key = (entity*)realloc(map->key, (last_element) * sizeof(entity));
    map->value = (struct sprite2d_batch_complex*)realloc(map->value, (last_element) * sizeof(struct sprite2d_batch_complex));

    map->size--;
}

void sprite2d_batch_complex_hashmap_free(struct sprite2d_batch_complex_hashmap* map)
{
    logger_log_string(WARNING, "Freeing sprite2D map memory!!\n");

    for (uint32_t i = 0; i < map->size; i++)
    {
        glDeleteBuffers(1, &map->value[i].vbo);
        glDeleteBuffers(1, &map->value[i].ebo);
        glDeleteVertexArrays(1, &map->value[i].vao);

        free(map->value[i].vertices);
        free(map->value[i].indices);
    }

    free(map->key);
    free(map->value);
}

struct sprite2d_batch_complex* sprite2d_batch_complex_hashmap_get(struct sprite2d_batch_complex_hashmap* map, entity key)
{
    for (uint32_t i = 0; i < map->size; i++)
        if (map->key[i] == key)
            return &map->value[i];

    return NULL;
}

