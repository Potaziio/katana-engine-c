#ifndef FONT_H
#define FONT_H

#include "../texture/texture.h"
#include "../utils/color.h"
#include "../utils/io_utils.h"
#include "render.h"
#include <string.h>

#define FONT_MAX_STRING_SIZE 150

extern struct font* _engine_default_font;

struct font_sprite_vertex{
    struct vector2 position;
    struct vector2 tex_coords;
    struct rgba_color color;
};

struct font
{
    // Holds data of fnt file
    int32_t data[950];
    int32_t image_width, image_height;
    int32_t char_padding;
    struct texture* bitmap;
    float line_height;
};

struct ui_text
{
    // Actual text stuff
    struct font* font;
    char* str;
    size_t str_len;
    struct vector2 position;
    float scale; // Scale of each letter

    struct rgba_color color;

    // OpenGL rendering stuff
    uint32_t vao, ebo, vbo;
    struct font_sprite_vertex* vertices;
    uint32_t* indices;
};

struct ui_text* ui_text_init(struct font* font, struct rgba_color color, struct vector2 position, float scale, const char* str, size_t len);
void ui_font_get_fnt_data(struct font* font, char* file);
void ui_text_mod(struct ui_text* text, struct rgba_color, const char* str, size_t length);
void ui_text_render(struct ui_text* text);
void ui_text_free(struct ui_text* text);

#endif

