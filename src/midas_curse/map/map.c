#include "map.h"

struct map* map_create(int expand_left, int expand_right, struct vector2 chunk_origin)
{
    struct map* map = (struct map*)malloc(sizeof(struct map));

    if (map == NULL)
    {
        logger_log_string(ERROR, "Error allocating memory for map\n");
        return NULL;
    }

    map->chunks = (struct chunk**)malloc(sizeof(struct chunk) * expand_left + expand_right);

    if (map->chunks == NULL)
    {
        logger_log_string(ERROR, "Error allocating chunk memory for map\n");
        return NULL;
    }

    map->chunk_num = 0;

    for (int i = -expand_left; i < expand_right; i++)
    {
        map->chunks[map->chunk_num] = chunk_create(vector2(chunk_origin.x + (i * CHUNK_TILE_SCALE * CHUNK_WIDTH), chunk_origin.y));
        map->chunk_num++;
    }

    return map;
}

void map_end(struct map* map)
{
    for (int i = 0; i < map->chunk_num; i++)
        free(map->chunks[i]);

    free(map->chunks);

    free (map);
}

