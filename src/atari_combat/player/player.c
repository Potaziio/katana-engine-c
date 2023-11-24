#include "player.h"

combat_player* player_create(struct vector2 origin, float rot)
{
    combat_player* player = (combat_player*)malloc(sizeof(combat_player));

    player->id = engine_create_entity(global_engine->ecs, TRANSFORM | TEXTURED_SPRITE2D | SCRIPT);

    struct transform* t = ENTITY_GET_TRANSFORM(player->id);
    struct script* sc = ENTITY_GET_SCRIPT(player->id);

    player->invinsibility.target = PLAYER_INVINSIBILITY_DURATION_MS;
    player->shoot_timer.target = PLAYER_SHOOT_DELAY_MS;

    script_system_set_funcs(sc, NULL, player_update_behavior);
    script_system_set_data(sc, (void*)player);

    t->position = origin;
    t->scale = vector2(PLAYER_SCALE, PLAYER_SCALE);
    t->rotation_angle = rot;

    return player;
}

void player_update_behavior(entity id)
{
    struct transform* t = ENTITY_GET_TRANSFORM(id);
    struct script* sc = ENTITY_GET_SCRIPT(id);

    combat_player* player = (combat_player*)sc->data;

    player->collider.position = vector2(t->position.x - t->scale.x * 0.5f, t->position.y - t->scale.y * 0.5f);
    player->collider.scale = t->scale;

    int8_t rotation_input = input_get_key(player->rotate_right_input) ? 1.0f : input_get_key(player->rotate_left_input) ? -1.0f : 0.0f;
    uint8_t move_input = input_get_key(player->move_input) ? 1.0f : 0.0f;

    // locks our angle so it doesnt reset by going to negatives and go past 360
    if (t->rotation_angle < 0) t->rotation_angle = 360;
    if (t->rotation_angle > 360) t->rotation_angle = 0;

    player->dir_vec = vector2(sin(deg_2_rad(t->rotation_angle)), -cos(deg_2_rad(t->rotation_angle)));

    // Send this is to bind our shooting to our timer
    if (input_get_key_down(player->shoot_input) && !player->has_shot)
    {
        // Create a buillet 'player' as data
        bullet_create(t->position, player->dir_vec, (void*)player);
        audio_manager_play_and_forget_sound(_engine_default_audio_manager, "../src/assets/tank_shoot.wav");
        player->has_shot = 1;
    }

    // Update our shoot timer 

    if (player->has_shot && timer_add_respond_and_reset(&player->shoot_timer, global_engine->delta_time))
    {
        player->has_shot = 0;
        player->shoot_timer.timer = 0;
    }

    // Player movement
    float desired_x = player->dir_vec.x * move_input * PLAYER_MOVE_SPEED * global_engine->delta_time;
    float desired_y = player->dir_vec.y * move_input * PLAYER_MOVE_SPEED * global_engine->delta_time;

    for (int i = 0; i < player->map->collider_size; i++)
    {
        struct aabb x_move;
        x_move.position = vector2(player->collider.position.x + desired_x, player->collider.position.y);
        x_move.scale = player->collider.scale;

        if (physics_aabb_in_aabb(player->map->collider[i], x_move))
            desired_x = 0.0f;

        struct aabb y_move;
        y_move.position = vector2(player->collider.position.x, player->collider.position.y + desired_y);
        y_move.scale = player->collider.scale;

        if (physics_aabb_in_aabb(player->map->collider[i], y_move))
            desired_y = 0.0f;
    }

    t->position.x += desired_x; 
    t->position.y += desired_y;

    // Make our player invinsible for a few frames 

    if (player->is_invinsible)
    {
        t->rotation_angle += global_engine->delta_time * 1000;

        if (timer_add_respond_and_reset(&player->invinsibility, global_engine->delta_time))
        {
            player->is_invinsible = 0;
            player->invinsibility.timer = 0;
        }
    }

    t->rotation_angle += rotation_input * global_engine->delta_time * PLAYER_ROTATION_SPEED;
}

void bullet_create(const struct vector2 origin, struct vector2 dir, void* data)
{
    entity bullet = engine_create_entity(global_engine->ecs, TRANSFORM | SPRITE2D | SCRIPT);

    struct transform* t = ENTITY_GET_TRANSFORM(bullet);
    struct script* sc = ENTITY_GET_SCRIPT(bullet);
    struct sprite2d* sp = ENTITY_GET_SPRITE2D(bullet);

    combat_player* d = (combat_player*)data;

    struct bullet_data* send = malloc(sizeof(struct bullet_data));

    send->player = d;
    send->dir = dir;

    t->position = origin;
    t->scale = vector2(4, 4);
    sp->color = rgba_color(RGBA_BLACK);
    sp->config = SPRITE2D_ENABLED | SPRITE2D_CENTERED;

    script_system_set_funcs(sc, NULL, bullet_update_behavior);
    script_system_set_data(sc, (void*)send);
}

void bullet_update_behavior(entity id)
{
    struct transform* t = ENTITY_GET_TRANSFORM(id);
    struct script* sc = ENTITY_GET_SCRIPT(id);
        
    struct bullet_data* data = (struct bullet_data*)sc->data;
    combat_player* target = (combat_player*)data->player->target_data.player;

    struct aabb bullet_collider;

    bullet_collider.position = vector2(t->position.x - t->scale.x * 0.5f, t->position.y - t->scale.y * 0.5f);
    bullet_collider.scale = t->scale;

    t->position.x += data->dir.x * BULLET_TRAVEL_SPEED * global_engine->delta_time;
    t->position.y += data->dir.y * BULLET_TRAVEL_SPEED * global_engine->delta_time;

    t->rotation_angle += data->dir.x * 900 * global_engine->delta_time;

    if (physics_aabb_in_aabb(bullet_collider, target->collider) && !target->is_invinsible)
    {
        data->player->score++;
        target->is_invinsible = 1;
        struct transform* target_pos = ENTITY_GET_TRANSFORM(target->id);
        target_pos->position = target->spawn_origin;

        audio_manager_play_and_forget_sound(_engine_default_audio_manager, "../src/assets/tank_die.wav");
        
        engine_pop_entity(global_engine->ecs, id);
        free(data);
        return;
    }

    if (t->position.x < 0.0f || t->position.x > engine_camera.bounds.x || 
        t->position.y < 0.0f || t->position.y > engine_camera.bounds.y)
    {
        
        engine_pop_entity(global_engine->ecs, id);
        free(data);
        return;
    }

    for (int i = 0; i < data->player->map->collider_size; i++)
    {
        if (physics_aabb_in_aabb(bullet_collider, data->player->map->collider[i]))
        {
            engine_pop_entity(global_engine->ecs, id);
            free(data);
            return;
        }
    }
}

