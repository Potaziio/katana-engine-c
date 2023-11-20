#ifndef LINE_H
#define LINE_H

#include "../../engine/core/engine.h"
#include "../../engine/math/vector/vector2.h"

#define LINE_STEP_SIZE 5.0f
#define LINE_MAX_SIZE 200

struct line
{
    struct vector2 p1;
    struct vector2 p2;
    float width;
    float line_points;
    uint32_t id;
    struct rgba_color color;
};

struct line line_create(struct vector2 p1, struct vector2 p2, struct rgba_color color, float width);
struct line line_update(struct line* line);
void line_destroy(uint32_t id);

#endif

