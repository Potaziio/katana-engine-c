#include "player.h"

struct texture player_texture;
struct timer powerup_timer;
struct timer score_timer;
char score_str[20];

player player_create()
{
    player p;

    p.id = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D);
    struct transform* t = ENTITY_GET_TRANSFORM(p.id);
    struct textured_sprite2d* spr = ENTITY_GET_TEXTURED_SPRITE2D(p.id);

    t->position = vector2(PLAYER_X_ORIGIN, engine_camera.bounds.y * 0.5f);
    t->scale = vector2(PLAYER_SCALE, PLAYER_SCALE);

    p.collider.position = vector2((t->position.x + 20)- t->scale.x * 0.5f, t->position.y - t->scale.y * 0.5f);
    p.collider.scale = vector2(t->scale.x, t->scale.y - 15.0f);

    spr->texture = &player_texture;

    p.score = 0;
    p.score_text = ui_text_init(_engine_default_font, (struct rgba_color){RGBA_WHITE}, vector2(t->position.x - 10, t->position.y - PLAYER_SCALE), 0.18f, "0", 1);
    p.dead = 0;
    powerup_timer.target = 1000;
    score_timer.target = 1100;

    return p;
}



void player_update(player* p)
{
    if (p->dead) return;

    sprintf(score_str, "%d", p->score);

    ui_text_mod(p->score_text, p->score_text->color, score_str, strlen(score_str));

    struct transform* t = ENTITY_GET_TRANSFORM(p->id);
    
    p->collider.position = vector2((t->position.x) - t->scale.x * 0.5f, (t->position.y + 8) - t->scale.y * 0.5f);
    p->score_text->position = vector2(t->position.x - 10, t->position.y - PLAYER_SCALE);

    if (input_get_key_down(GLFW_KEY_SPACE) || input_get_mouse_button_down(0))
    {
        audio_manager_play_and_forget_sound(_engine_default_audio_manager, "../src/assets/jump.wav");
        p->velocity.y = 0;
        p->velocity.y = PLAYER_JUMP_POWER;
    }

    if (timer_add_respond_and_reset(&score_timer, global_engine->delta_time))
        p->score += 1;

    p->velocity.y += GRAVITY * global_engine->delta_time; 
    t->position.y += p->velocity.y * global_engine->delta_time;

    t->rotation_angle = p->velocity.y * 0.1f;

    if (t->position.y >= engine_camera.bounds.y)
        p->dead = 1;

    player_powerup(p, &powerup_timer);
}

void player_powerup(player* p, struct timer* timer)
{
    int timeup = timer_add_and_respond(timer, global_engine->delta_time);

    // Every multiple of POWERUP_TIME we play a sound and add POWERUP_BONUS to our score,
    // we then have to wait 1100 ms for a timer because we dont want our sound to keep playing 
    // or our score to keep adding 
    if (p->score > 0 && p->score % POWERUP_TIME == 0 && timeup)
    {
        audio_manager_play_and_forget_sound(_engine_default_audio_manager, "../src/assets/powerup.wav");
        timer_reset(&powerup_timer);
        p->score += POWERUP_BONUS;
    }
}


void player_die(player* p)
{
    audio_manager_play_and_forget_sound(_engine_default_audio_manager, "../src/assets/die.wav");
    p->dead = 1;
}

