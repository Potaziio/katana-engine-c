#ifndef PLAYER_H
#define PLAYER_H

#include "../../engine/core/engine.h"
#include "../../engine/math/math.h"

#define PLAYER_ROTATION_SPEED 80 
#define PLAYER_MOVE_SPEED 100

#define BULLET_TRAVEL_SPEED 250

typedef struct 
{
    entity id;

    struct aabb collider;
    struct vector2 dir;
} combat_bullet;

typedef struct 
{  
    entity id;

    struct vector2 dir_vec;
    struct aabb collider;

    float dir_angle;
    uint32_t rotate_left_input;
    uint32_t rotate_right_input;
    uint32_t move_input;
    uint32_t shoot_input;

} combat_player;

combat_player* player_create(struct vector2 origin, float rot);
void player_update_behavior(entity id);

void bullet_create(struct vector2 origin, struct vector2 dir);
void bullet_update_behavior(entity id);

#endif

