#include "line.h"
#include <complex.h>
#include <stdlib.h>

void line_create_verts(struct sprite2d_batch_simple* batch, struct vector2 p1, struct vector2 dir, float width, float dist, struct rgba_color color)
{
    size_t v_cursor = 0;
    size_t i_cursor = 0;
    size_t ind_r_cursor = 0;

    for (float i = 0.0f; i < dist; i += LINE_STEP_SIZE)
    {
        struct vector2 start = vector2(p1.x + (dir.x * i), p1.y + (dir.y * i));

        batch->vertices[v_cursor].position = start;
        batch->vertices[v_cursor + 1].position = vector2(start.x + width, start.y);
        batch->vertices[v_cursor + 2].position = vector2(start.x, start.y + width);
        batch->vertices[v_cursor + 3].position = vector2(start.x + width, start.y + width);

        batch->vertices[v_cursor].color = color; 
        batch->vertices[v_cursor + 1].color = color;
        batch->vertices[v_cursor + 2].color = color;
        batch->vertices[v_cursor + 3].color = color;

        batch->indices[i_cursor] = 0 + ind_r_cursor;
        batch->indices[i_cursor + 1] = 1 + ind_r_cursor;
        batch->indices[i_cursor + 2] = 2 + ind_r_cursor;
        batch->indices[i_cursor + 3] = 2 + ind_r_cursor;
        batch->indices[i_cursor + 4] = 3 + ind_r_cursor;
        batch->indices[i_cursor + 5] = 1 + ind_r_cursor;

        v_cursor += 4;
        i_cursor += 6;
        ind_r_cursor += 4;
    }
}

struct line line_create(struct vector2 p1, struct vector2 p2, struct rgba_color color, float width)
{
    struct line line;
    line.id = engine_create_entity(global_engine->ecs, SPRITE2D_BATCH_SIMPLE);
    struct sprite2d_batch_simple* batch = ENTITY_GET_SPRITE2D_BATCH_SIMPLE(line.id);

    line.p1 = p1;
    line.p2 = p2;
    line.width = width;
    line.color = color;

    struct vector2 dir = vector2(p2.x - p1.x, p2.y - p1.y);
    vector2_normalize(&dir);

    float dist = vector2_dist(p1, p2);

    // Round up in case and add one in case it gets rounded down
    batch->batch_size = ceil(dist / LINE_STEP_SIZE);

    size_t vertex_size = batch->batch_size * SPRITE2D_VERTEX_NUM * sizeof(struct rectangle_sprite_vertex);
    size_t index_size = batch->batch_size * SPRITE2D_INDEX_NUM * sizeof(uint32_t);

    batch->vertices = (struct rectangle_sprite_vertex*)malloc(vertex_size);
    batch->indices = (uint32_t*)malloc(index_size);

    line_create_verts(batch, line.p1, dir, width, dist, color);
    
    return line;
}

struct line line_update(struct line* line)
{
    logger_log_string(ERROR, "Unimplemented\n");

    return (struct line){0};

    struct line result = line_create(line->p1, line->p2, line->color, line->width);
    line_destroy(line->id);
    return result;
}

void line_destroy(uint32_t id)
{ engine_pop_entity(global_engine->ecs, id); }
