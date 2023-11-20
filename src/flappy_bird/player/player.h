#ifndef PLAYER_H
#define PLAYER_H

#include "../../engine/core/engine.h"

#define GRAVITY 1200.0f
#define PLAYER_X_ORIGIN 400
#define PLAYER_SCALE 42
#define PLAYER_JUMP_POWER -430
#define POWERUP_TIME 10
#define POWERUP_BONUS 2

extern struct texture player_texture;

typedef struct 
{
    entity id;
    struct vector2 velocity;
    struct aabb collider;
    float speed;
    uint32_t score;
    struct ui_text* score_text;
    uint8_t dead;
} player;

player player_create();
void player_update(player* p);
void player_die(player* p);
void player_powerup(player* p, struct timer* timer);

#endif

