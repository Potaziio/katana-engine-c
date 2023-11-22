#include "player.h"

combat_player* player_create(struct vector2 origin, float rot)
{
    combat_player* player = (combat_player*)malloc(sizeof(combat_player));

    player->id = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D | SCRIPT);

    struct transform* t = ENTITY_GET_TRANSFORM(player->id);
    struct script* sc = ENTITY_GET_SCRIPT(player->id);

    script_system_set_funcs(sc, NULL, player_update_behavior);
    script_system_set_data(sc, (void*)player);

    t->position = origin;
    t->scale = vector2(32.0f, 32.0f);
    t->rotation_angle = rot;

    return player;
}

void player_update_behavior(entity id)
{
    struct transform* t = ENTITY_GET_TRANSFORM(id);
    struct script* sc = ENTITY_GET_SCRIPT(id);

    combat_player* player = (combat_player*)sc->data;

    int8_t rotation_input = input_get_key(player->rotate_right_input) ? 1.0f : input_get_key(player->rotate_left_input) ? -1.0f : 0.0f;
    uint8_t move_input = input_get_key(player->move_input) ? 1.0f : 0.0f;

    // locks our angle so it doesnt reset by going to negatives and go past 360
    if (t->rotation_angle < 0) t->rotation_angle = 360;
    if (t->rotation_angle > 360) t->rotation_angle = 0;

    player->dir_vec = vector2(sin(deg_2_rad(t->rotation_angle)), -cos(deg_2_rad(t->rotation_angle)));

    if (input_get_key_down(player->shoot_input))
        bullet_create(t->position, player->dir_vec);

    t->position.x += player->dir_vec.x * move_input * PLAYER_MOVE_SPEED * global_engine->delta_time;
    t->position.y += player->dir_vec.y * move_input * PLAYER_MOVE_SPEED * global_engine->delta_time;

    t->rotation_angle += rotation_input * global_engine->delta_time * PLAYER_ROTATION_SPEED;
}

void bullet_create(struct vector2 origin, struct vector2 dir)
{
    entity bullet = engine_create_entity(global_engine, TRANSFORM | SPRITE2D | SCRIPT);

    struct transform* t = ENTITY_GET_TRANSFORM(bullet);
    struct script* sc = ENTITY_GET_SCRIPT(bullet);
    struct sprite2d* sp = ENTITY_GET_SPRITE2D(bullet);

    /* combat_bullet* bul = (combat_bullet*)malloc(sizeof(combat_bullet)); */

    /* bul->dir = dir; */

    t->position = origin;
    t->scale = vector2(4.0f, 4.0f);
    sp->color = rgba_color(RGBA_BLACK);
    sp->config = SPRITE2D_ENABLED | SPRITE2D_CENTERED;

    script_system_set_funcs(sc, NULL, bullet_update_behavior);
    /* script_system_set_data(sc, (void*)bul); */
}

void bullet_update_behavior(entity id)
{
    struct transform* t = ENTITY_GET_TRANSFORM(id);

    t->position.x += 10.0f * global_engine->delta_time;
    /* struct script* sc = ENTITY_GET_SCRIPT(id); */

    /* combat_bullet* bullet = (combat_bullet*)sc->data; */

    /* t->position.x += bullet->dir.x * BULLET_TRAVEL_SPEED * global_engine->delta_time; */
    /* t->position.y += bullet->dir.y * BULLET_TRAVEL_SPEED * global_engine->delta_time; */

    if (t->position.x < 0.0f || t->position.x > engine_camera.bounds.x || 
        t->position.y < 0.0f || t->position.y > engine_camera.bounds.y)
    {
        engine_pop_entity(global_engine, id);
        /* free(bullet); */
    }
}

