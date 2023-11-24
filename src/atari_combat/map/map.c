#include "map.h"

combat_map* combat_map_create(char* file)
{       
    combat_map* map = (combat_map*)malloc(sizeof(combat_map));

    // Read contents of file 

    char buf[1024];
    memset(buf, '0', 1024);

    size_t size =  io_utils_read_file(file, buf);

    int32_t ptr = 0;
        
    for (size_t i = 0; i < size; i++)
    {
        if (buf[i] - '0' > -1)
        {
            map->tiles[ptr] = buf[i] - '0';
            ptr++;
        }
    }

    map->id = engine_create_entity(global_engine->ecs, TRANSFORM | SPRITE2D_BATCH_SIMPLE);

    struct sprite2d_batch_simple* batch = ENTITY_GET_SPRITE2D_BATCH_SIMPLE(map->id);

    batch->batch_size = COMBAT_MAP_ROWS * COMBAT_MAP_COLS;
    batch->vertices = (struct rectangle_sprite_vertex*)malloc(sizeof(struct rectangle_sprite_vertex) * 4 * batch->batch_size);
    batch->indices = (uint32_t*)malloc(sizeof(uint32_t) * 6 * batch->batch_size);

    uint32_t v_cursor = 0;
    uint32_t i_cursor = 0;
    uint32_t id_cursor = 0;
    uint32_t collider_cursor = 0;

    for (int y = 0; y < COMBAT_MAP_ROWS; y++)
    {
        for (int x = 0; x < COMBAT_MAP_COLS; x++)
        {
            if (map->tiles[y * COMBAT_MAP_COLS + x] == 0) continue;

            struct vector2 beg = vector2(x * 32.0f, y * 32.0f);

            batch->vertices[v_cursor].position = beg; 
            batch->vertices[v_cursor + 1].position = vector2(beg.x + 32.0f, beg.y);
            batch->vertices[v_cursor + 2].position = vector2(beg.x, beg.y + 32.0f);
            batch->vertices[v_cursor + 3].position = vector2(beg.x + 32.0f, beg.y + 32.0f);

            map->collider_size++;

            map->collider[collider_cursor].position = beg;
            map->collider[collider_cursor].scale = vector2(32.0f, 32.0f);

            batch->vertices[v_cursor].color = rgba_color(234, 151, 91, 255);
            batch->vertices[v_cursor + 1].color = rgba_color(234, 151, 91, 255);
            batch->vertices[v_cursor + 2].color = rgba_color(234, 151, 91, 255);
            batch->vertices[v_cursor + 3].color = rgba_color(234, 151, 91, 255);

            batch->indices[i_cursor] = 0 + id_cursor;
            batch->indices[i_cursor + 1] = 1 + id_cursor;
            batch->indices[i_cursor + 2] = 2 + id_cursor;
            batch->indices[i_cursor + 3] = 2 + id_cursor;
            batch->indices[i_cursor + 4] = 3 + id_cursor;
            batch->indices[i_cursor + 5] = 1 + id_cursor;

            v_cursor += 4;
            i_cursor += 6;
            id_cursor += 4;
            collider_cursor++;
        }
    }

    return map;
}
            
