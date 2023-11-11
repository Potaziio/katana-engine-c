#ifndef FONT_H
#define FONT_H

#include "../texture/texture.h"
#include "../utils/color.h"
#include "render.h"
#include <string.h>

#define FONT_MAX_STRING_SIZE 150

extern struct font* _engine_default_font;

struct font_sprite_vertex{
    struct vector3 position;
    struct vector2 tex_coords;
    struct rgba_color color;
};

struct font
{
    int image_width, image_height;
    int scale;
    int char_padding;
    struct texture* bitmap;
};

struct ui_text
{
    // Actual text stuff
    struct font* font;
    char* str;
    int str_len;
    struct vector2 position;
    float scale; // Scale of each letter
    float full_scale; // Scale of the whole text

    struct rgba_color color;

    // OpenGL rendering stuff
    unsigned int vao, ebo, vbo;
    struct font_sprite_vertex* vertices;
    unsigned int* indices;
};

struct ui_text* ui_text_init(struct font* font, struct rgba_color color, struct vector2 position, float scale, const char* str, int len);
void ui_text_mod(struct ui_text* text, const char* str, int length);
void ui_text_render(struct ui_text* text);
void ui_text_free(struct ui_text* text);

#endif
