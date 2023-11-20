#include "map.h"
#include "chunk.h"

struct texture background_texture;

struct map* map_create(int expand_left, int expand_right, struct vector2 chunk_origin)
{
    struct map* map = (struct map*)malloc(sizeof(struct map));

    if (map == NULL)
    {
        logger_log_string(ERROR, "Error allocating memory for map\n");
        return NULL;
    }

    size_t chunks_byte_size = sizeof(struct chunk) * expand_left + expand_right;

    map->chunks = (struct chunk**)malloc(chunks_byte_size);

    if (map->chunks == NULL)
    {
        logger_log_string(ERROR, "Error allocating chunk memory for map\n");
        return NULL;
    }

    map->chunk_num = 0;

    for (int32_t i = -expand_left; i < expand_right; i++)
    {
        map->chunks[map->chunk_num] = chunk_create(vector2(chunk_origin.x + (i * CHUNK_TILE_SCALE * CHUNK_WIDTH), chunk_origin.y));
        map->chunk_num++;
    }

    /* map->background = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D); */
    /* struct transform* t = ENTITY_GET_TRANSFORM(map->background); */
    /* struct textured_sprite2d* s = ENTITY_GET_TEXTURED_SPRITE2D(map->background); */
    /* t->scale = vector2(engine_camera.bounds.x, engine_camera.bounds.y); */
    /* s->texture = &background_texture; */
    /* s->config = SPRITE2D_TOP_LEFT; */

    return map;
}

void map_end(struct map* map)
{
    for (uint32_t i = 0; i < map->chunk_num; i++)
        free(map->chunks[i]);

    free(map->chunks);
    free (map);
}

void map_chunk_update(struct map* map)
{
    map->rendered_chunks = 0;

    for (uint32_t i = 0; i < map->chunk_num; i++)
    {
        chunk_update(map->chunks[i]);
        if (map->chunks[i]->batch->active)
            map->rendered_chunks++;
    }
}

