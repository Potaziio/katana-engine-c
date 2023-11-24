#include "combat.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "player/player.h"
#include "map/map.h"

combat_game_data combat_data;

struct ui_text* red_score;
struct ui_text* blue_score;

combat_map* map;

void combat_start(void)
{
    global_engine->ecs->config = ENGINE_SILENCE_ENTITY_LOG;

    texture_load(&combat_data.red_tank_texture, "../src/assets/tank_red.png");
    texture_load(&combat_data.blue_tank_texture, "../src/assets/tank_blue.png");

    map = combat_map_create("../src/atari_combat/map/map_files/map1.cm");

    struct vector2 blue_spawn = vector2(engine_camera.bounds.x - 100.0f, engine_camera.bounds.y * 0.5f);
    struct vector2 red_spawn = vector2(100.0f, engine_camera.bounds.y * 0.5f);

    combat_data.player_red = player_create(red_spawn, 90.0f);
    combat_data.player_blue = player_create(blue_spawn, 270.0f);

    combat_data.player_red->target_data.player = combat_data.player_blue;
    combat_data.player_blue->target_data.player = combat_data.player_red;
    combat_data.player_blue->map = map;
    combat_data.player_blue->spawn_origin = blue_spawn;

    combat_data.player_blue->rotate_left_input = GLFW_KEY_LEFT;
    combat_data.player_blue->rotate_right_input = GLFW_KEY_RIGHT;
    combat_data.player_blue->move_input = GLFW_KEY_UP;
    combat_data.player_blue->shoot_input = GLFW_KEY_RIGHT_SHIFT;

    combat_data.player_red->rotate_left_input = GLFW_KEY_A;
    combat_data.player_red->rotate_right_input = GLFW_KEY_D;
    combat_data.player_red->move_input = GLFW_KEY_W;
    combat_data.player_red->shoot_input = GLFW_KEY_E;
    combat_data.player_red->map = map;
    combat_data.player_red->spawn_origin = red_spawn;

    (ENTITY_GET_TEXTURED_SPRITE2D(combat_data.player_blue->id))->texture = &combat_data.blue_tank_texture;
    (ENTITY_GET_TEXTURED_SPRITE2D(combat_data.player_red->id))->texture = &combat_data.red_tank_texture;

    red_score = ui_text_init(_engine_default_font, rgba_color(RGBA_RED), vector2(100.0, 10.0f), 0.5f, "0", 1);
    blue_score = ui_text_init(_engine_default_font, rgba_color(RGBA_BLUE), vector2(engine_camera.bounds.x - 100.0f, 10.0f), 0.5f, "0", 1);

}

void combat_update(void)
{
    if (combat_data.player_blue->is_invinsible)
    {
        char red_score_text[2];
        sprintf(red_score_text, "%d", combat_data.player_red->score);
        ui_text_mod(red_score, red_score->color, red_score_text, strlen(red_score_text)); 
    }

    if (combat_data.player_red->is_invinsible)
    {
        char blue_score_text[2];
        sprintf(blue_score_text, "%d", combat_data.player_blue->score);
        ui_text_mod(blue_score, blue_score->color, blue_score_text, strlen(blue_score_text)); 
    }

    ui_text_render(red_score);
    ui_text_render(blue_score);
}

void combat_end(void)
{
    free(combat_data.player_red);
    free(combat_data.player_blue);
    texture_free(&combat_data.blue_tank_texture);
    texture_free(&combat_data.red_tank_texture);
    ui_text_free(red_score);
    ui_text_free(blue_score);
    free(map);
}

