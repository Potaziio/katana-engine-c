// This creates only and only centered menus 

#ifndef UI_MENU_H
#define UI_MENU_H

#include "../../core/engine.h"

#include "../../math/physics/shape/shapes.h"
#include "../../render/font.h"
#include "../../input/input.h"
#include "../../audio/audio_manager.h"
#include "../../math/physics/physics.h"
#include "../../camera/camera.h"

#define UI_MENU_ELEMENT_STATIC_COLOR (struct rgba_color){RGBA_BLACK}
#define UI_MENU_ELEMENT_ON_HOVER_COLOR (struct rgba_color){RGBA_RED}

#define UI_MENU_DEFAULT_TEXT_SCALE 32.0f
#define UI_MENU_DEFAULT_VERTICAL_OPTION_PADDING 5.0f;

// Menu flags

#define UI_MENU_DISABLED 0 
#define UI_MENU_ACTIVE 1

#define UI_MENU_CENTERED 1
#define UI_MENU_ALIGNED 2
#define UI_MENU_HEADER 4
#define UI_MENU_DOUBLE_SPACED 8

struct ui_menu_option 
{
    struct ui_text* text;
    struct aabb collider;
    unsigned char flags;
};

struct ui_menu 
{
    struct ui_menu_option* options;

    int option_num;
    float bottom_padding;
    float horizontal_padding;
    float vertical_padding;
    unsigned char option_click;
    unsigned char active;
};

int ui_menu_get_option_on_click(struct ui_menu* menu);
void ui_menu_allocate(struct ui_menu* menu, int option_size);
void ui_menu_begin_option(struct ui_menu* menu, char* text, int option, unsigned char flags);
void ui_menu_fix_resize(struct ui_menu* menu);
void ui_menu_render(struct ui_menu* menu);
void ui_menu_free(struct ui_menu* menu);

#endif

