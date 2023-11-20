#include "ui_menu.h"
#include "GLFW/include/GLFW/glfw3.h"
#include <complex.h>

void ui_menu_allocate(struct ui_menu* menu, int option_size)
{
    // Allocate memory and set some starting values

    menu->option_num = option_size;

    menu->options = (struct ui_menu_option*)malloc(sizeof(struct ui_menu_option) * menu->option_num);

    if (menu->options == NULL)
    {
        logger_log_string(ERROR, "Failed to allocate ui menu options memory!\n");
        return;
    }

    if (menu->vertical_padding == 0.0f)
        menu->vertical_padding = UI_MENU_DEFAULT_VERTICAL_OPTION_PADDING;

    menu->option_click = -1;
}

void ui_menu_calculate_option_pos(struct ui_menu* menu, int option, int length, float* x, float* y)
{
    unsigned char flags = menu->options[option].flags;

    float text_x = 0.0f;

    // If its centered we center it to the screen
    // We do, ((x bounds / 2) - (the length of the text * its scale) / 2)
    // If its aligned we set it to the horizontal_padding of the menu
        
    if (flags & UI_MENU_CENTERED)
        text_x = engine_camera.bounds.x * 0.5f - ((length * UI_MENU_DEFAULT_TEXT_SCALE) * 0.5f);
    else if (flags & UI_MENU_ALIGNED)
        text_x = menu->horizontal_padding;

    int is_double_spaced = (flags & UI_MENU_DOUBLE_SPACED) > 1;
    // Defines if the current option is to be double spaced
    int is_last_double_spaced = option > 0 ? (menu->options[option - 1].flags & UI_MENU_DOUBLE_SPACED) > 1 : 0;

    // If its our first option, we just set it to the bounds of our camera - the bottom padding, 
    // If it isnt, we set it below the option above
    float text_y = option > 0 ? menu->options[option - 1].text->position.y + menu->vertical_padding + (UI_MENU_LINE_HEIGHT * UI_MENU_DEFAULT_TEXT_SCALE) + (is_double_spaced * UI_MENU_DEFAULT_TEXT_SCALE) : engine_camera.bounds.y - menu->bottom_padding;

    // Creates a collider for our option, we substract 15 pixels to make it aligned with the text

    if (!(flags & UI_MENU_HEADER))
    {
        menu->options[option].collider.position = (struct vector2){text_x, text_y + (is_last_double_spaced * UI_MENU_DEFAULT_TEXT_SCALE)};
        menu->options[option].collider.scale = (struct vector2){((length * UI_MENU_DEFAULT_TEXT_SCALE)) - 15.0f, UI_MENU_DEFAULT_TEXT_SCALE};
    }

    *x = text_x;
    *y = text_y;
}

void ui_menu_begin_option(struct ui_menu* menu, char* text, int option, unsigned char flags)
{
    float text_x = 0.0f;
    float text_y = 0.0f;

    menu->options[option].flags = flags;

    // We calculate the position of our text

    ui_menu_calculate_option_pos(menu, option, strlen(text), &text_x, &text_y);  

    int is_last_double_spaced = option > 0 ? (menu->options[option - 1].flags & UI_MENU_DOUBLE_SPACED) > 1 : 0;

    menu->options[option].text = ui_text_init
        (
            _engine_default_font, 
            UI_MENU_ELEMENT_STATIC_COLOR, 
            (struct vector2){text_x, text_y + (is_last_double_spaced * UI_MENU_DEFAULT_TEXT_SCALE)}, 
            UI_MENU_DEFAULT_TEXT_SCALE, text, 
            strlen(text)
        );
}

int ui_menu_get_option_on_click(struct ui_menu* menu)
{
    if (menu->active && (input_get_key_down(GLFW_KEY_ENTER) || input_get_key_down(GLFW_KEY_SPACE)) && menu->option_click > -1)
        return menu->option_click; 
    else 
        return -1;
}

void ui_menu_render(struct ui_menu* menu)
{
    if (!menu->active) return;

    struct rgba_color text_color;

    // We go down or up on our menu list

    if (input_get_key_down(GLFW_KEY_DOWN) && menu->option_click < (int8_t)menu->option_num - 1)
        menu->option_click++;
    else if (input_get_key_down(GLFW_KEY_UP) && menu->option_click > 0)
        menu->option_click--;

    for (uint32_t i = 0; i < menu->option_num; i++)
    {
        text_color = UI_MENU_ELEMENT_STATIC_COLOR;

        if (menu->option_click == (int8_t)i)
            text_color = UI_MENU_ELEMENT_ON_HOVER_COLOR;

        char* cpy = (char*)malloc(sizeof(char) * menu->options[i].text->str_len);
        memcpy(cpy, menu->options[i].text->str, sizeof(char) * menu->options[i].text->str_len);

        /*
        if (physics_point_in_aabb(input_get_mouse_pos(), menu->options[i].collider))
        {

            if (input_get_mouse_button_down(GLFW_MOUSE_BUTTON_LEFT))
            {
                // audio_manager_play_and_forget_sound(_engine_default_audio_manager, "../src/app/assets/sounds/jump.wav");
                menu->option_click = i;
            }
        }
        */

        ui_text_mod(menu->options[i].text, text_color, cpy, menu->options[i].text->str_len);
        ui_text_render(menu->options[i].text);

        free(cpy);
    }   
}

void ui_menu_fix_resize(struct ui_menu* menu)
{   
    for (uint32_t i = 0; i < menu->option_num; i++)
    {
        float text_y = 0.0f;
        float text_x = 0.0f;

        int is_last_double_spaced = i > 0 ? (menu->options[i - 1].flags & UI_MENU_DOUBLE_SPACED) > 1 : 0;

        ui_menu_calculate_option_pos(menu, i, menu->options[i].text->str_len, &text_x, &text_y);

        menu->options[i].text->position = (struct vector2){text_x, text_y + (is_last_double_spaced * UI_MENU_DEFAULT_TEXT_SCALE)};

        char* cpy = malloc(sizeof(char) * menu->options[i].text->str_len);
        memcpy(cpy, menu->options[i].text->str, sizeof(char) * menu->options[i].text->str_len);
        ui_text_mod(menu->options[i].text, menu->options[i].text->color, cpy, menu->options[i].text->str_len);
        free(cpy);
    }
}

void ui_menu_free(struct ui_menu* menu)
{
    for (uint32_t i = 0; i < menu->option_num; i++)
    {
        ui_text_free(menu->options[i].text);
    }

    free(menu->options);
}

