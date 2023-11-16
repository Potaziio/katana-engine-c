#ifndef ENTITY_H
#define ENTITY_H

#define TRANSFORM 1 // 2^0
#define RIGIDBODY 2 // 2^1
#define SPRITE2D 4 // 2^2
#define TEXTURED_SPRITE2D 8 // 2^3
#define SPRITE2D_BATCH_SIMPLE 16 // 2^4
#define DEBUG_LINE 32 // 2^5
#define SCRIPT 64 // 2^6
#define SPRITE2D_BATCH_COMPLEX 128 // 2^7

#define SPRITE2D_CENTERED 1
#define SPRITE2D_TOP_LEFT 2
#define SPRITE2D_WIREFRAME 4

#include "../math/vector/vector2.h"
#include "../math/vector/vector3.h"
#include "../utils/color.h"
#include "../texture/texture.h"

#include "../../../include/cglm/include/cglm/mat4.h"

typedef uint32_t entity;

struct transform 
{
    struct vector2 position;
    struct vector2 scale;
    float32_t rotation_angle;
    uint8_t rotation_z;
    mat4 matrix;
};

struct rectangle_sprite_vertex 
{
    struct vector2 position;
    struct rgba_color color;
};

struct sprite2d 
{
    uint32_t vao, vbo, ebo;
    struct rgba_color color;
    struct rectangle_sprite_vertex vertices[4];
    uint32_t indices[6];
    int8_t was_initialized;
    uint8_t config;

};

struct line_vertex
{
    struct vector2 position;
    struct rgba_color color;
};

struct debug_line
{
    uint32_t vao, vbo;
    struct line_vertex vertices[2];
    uint8_t was_initialized;
};

struct textured_rectangle_sprite_vertex
{
    struct vector2 position;
    struct vector2 tex_coords;
};

struct textured_sprite2d
{
    uint32_t vao, vbo, ebo;
    struct textured_rectangle_sprite_vertex vertices[4];
    uint32_t indices[6];
    struct texture* texture;
    uint8_t was_initialized;
    uint8_t config;
};

struct sprite2d_batch_simple 
{
    uint32_t vao, vbo, ebo;
    struct rectangle_sprite_vertex* vertices;
    uint32_t* indices;
    struct rgba_color color;
    uint32_t batch_size;
    uint8_t was_initialized;
};

struct sprite2d_batch_complex
{
    uint32_t vao, vbo, ebo;
    struct textured_rectangle_sprite_vertex* vertices;
    uint32_t* indices;
    struct texture* atlas;
    uint32_t batch_size;
    uint8_t was_initialized;
};

struct script 
{
    entity entity;
    void* data;
    void (*start)(entity e);
    void (*update)(entity e);
};

#endif

