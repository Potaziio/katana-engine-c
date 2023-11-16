#include "chunk.h"
#include <iso646.h>

struct texture map_atlas;

struct chunk* chunk_create(struct vector2 origin)
{
    logger_log_string(LOG, "Attempting to create chunk at: ");
    printf("(%f, %f)\n", origin.x, origin.y);
    
    struct chunk* chunk = (struct chunk*)malloc(sizeof(struct chunk));
    if (chunk == NULL)
    {
        logger_log_string(ERROR, "Error allocating memory for chunk\n");
        return NULL;
    }

    chunk->id = engine_create_entity(global_engine, SPRITE2D_BATCH_COMPLEX);
    chunk->batch = ENTITY_GET_SPRITE2D_BATCH_COMPLEX(chunk->id);
    chunk->batch->batch_size = CHUNK_WIDTH * CHUNK_HEIGHT;
    chunk->batch->vertices = (struct textured_rectangle_sprite_vertex*)malloc(sizeof(struct textured_rectangle_sprite_vertex) * CHUNK_WIDTH * CHUNK_HEIGHT * SPRITE2D_VERTEX_NUM);
    chunk->batch->indices = (unsigned int*)malloc(sizeof(unsigned int) * CHUNK_WIDTH * CHUNK_HEIGHT * SPRITE2D_INDEX_NUM);
    chunk->batch->atlas = &map_atlas;

    int v_index = 0;
    int ind_index = 0;
    int ind_cursor = 0;

    float x_pos = origin.x;
    float y_pos = origin.y;

    float v_pos = 32.0f;

    for (int y = 0; y < CHUNK_HEIGHT; y++)
    {
        for (int x = 0; x < CHUNK_WIDTH; x++)
        {
            int is_rock = y > 0;
                
            chunk->batch->vertices[v_index].position = vector2(x_pos, y_pos);
            chunk->batch->vertices[v_index + 1].position = vector2(x_pos + CHUNK_TILE_SCALE, y_pos);
            chunk->batch->vertices[v_index + 2].position = vector2(x_pos, y_pos + CHUNK_TILE_SCALE);
            chunk->batch->vertices[v_index + 3].position = vector2(x_pos + CHUNK_TILE_SCALE, y_pos + CHUNK_TILE_SCALE);

            chunk->batch->vertices[v_index].tex_coords = 
                vector2(is_rock * (v_pos / (float)texture_get_width(map_atlas)), 
                        1.0f);

            chunk->batch->vertices[v_index + 1].tex_coords = 
                vector2((v_pos + (is_rock * CHUNK_TILE_SCALE)) / (float)texture_get_width(map_atlas), 
                        1.0f);

            chunk->batch->vertices[v_index + 2].tex_coords = 
                vector2(is_rock * (v_pos / (float)texture_get_width(map_atlas)), 
                        1.0f - (float)(32.0f / (float)texture_get_height(map_atlas)));

            chunk->batch->vertices[v_index + 3].tex_coords = 
                vector2((v_pos + (is_rock * CHUNK_TILE_SCALE)) / texture_get_width(map_atlas), 
                        1.0f - (float)(32.0f / (float)texture_get_height(map_atlas)));

            chunk->batch->indices[ind_index] = 0 + ind_cursor;
            chunk->batch->indices[ind_index + 1] = 1 + ind_cursor;
            chunk->batch->indices[ind_index + 2] = 2 + ind_cursor;
            chunk->batch->indices[ind_index + 3] = 2 + ind_cursor;
            chunk->batch->indices[ind_index + 4] = 3 + ind_cursor;
            chunk->batch->indices[ind_index + 5] = 1 + ind_cursor;

            ind_cursor += 4;
            v_index += 4;
            ind_index += 6;

            x_pos += 32.0f;
        }

        y_pos += 32.0f;
        x_pos = origin.x;
    }

    chunk->enabled = 1;

    return chunk;
}
