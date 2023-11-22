#include "combat.h"
#include "player/player.h"

combat_game_data combat_data;

void combat_start(void)
{
    global_engine->config = ENGINE_UPDATE_CAMERA_BOUNDS;

    texture_load(&combat_data.red_tank_texture, "../src/assets/tank_red.png");
    texture_load(&combat_data.blue_tank_texture, "../src/assets/tank_blue.png");

    combat_data.player_red = player_create(vector2(100.0f, engine_camera.bounds.y * 0.5f), 90.0f);
    combat_data.player_blue = player_create(vector2(engine_camera.bounds.x - 100.0f, engine_camera.bounds.y * 0.5f), 180.0f);

    combat_data.player_blue->rotate_left_input = GLFW_KEY_LEFT;
    combat_data.player_blue->rotate_right_input = GLFW_KEY_RIGHT;
    combat_data.player_blue->move_input = GLFW_KEY_UP;
    combat_data.player_blue->shoot_input = GLFW_KEY_SPACE;

    combat_data.player_red->rotate_left_input = GLFW_KEY_A;
    combat_data.player_red->rotate_right_input = GLFW_KEY_D;
    combat_data.player_red->move_input = GLFW_KEY_W;
    combat_data.player_red->shoot_input = GLFW_KEY_E;

    (ENTITY_GET_TEXTURED_SPRITE2D(combat_data.player_blue->id))->texture = &combat_data.blue_tank_texture;
    (ENTITY_GET_TEXTURED_SPRITE2D(combat_data.player_red->id))->texture = &combat_data.red_tank_texture;
}

void combat_update(void)
{
    
}

void combat_end(void)
{
    free(combat_data.player_red);
    free(combat_data.player_blue);
    texture_free(&combat_data.blue_tank_texture);
    texture_free(&combat_data.red_tank_texture);
}

