#ifndef PLAYER_H
#define PLAYER_H

#include "../../engine/core/engine.h"
#include "../../engine/math/math.h"

#include "../map/map.h"

#define PLAYER_ROTATION_SPEED 130 
#define PLAYER_MOVE_SPEED 100
#define BULLET_TRAVEL_SPEED 250
#define PLAYER_INVINSIBILITY_DURATION_MS 700
#define PLAYER_SHOOT_DELAY_MS 570
#define PLAYER_SCALE 48

typedef struct 
{
    entity id;

    struct aabb collider;
    struct vector2 dir;

} combat_bullet;

typedef struct 
{
    void* player;
} combat_hit_data;

typedef struct 
{ 
    entity id;

    struct vector2 dir_vec;
    struct aabb collider;
    struct timer invinsibility;
    struct timer shoot_timer;

    struct vector2 spawn_origin;

    float dir_angle;
    uint32_t rotate_left_input;
    uint32_t rotate_right_input;
    uint32_t move_input;
    uint32_t shoot_input;
    uint8_t score;
    uint8_t is_invinsible;
    uint8_t has_shot;

    combat_map* map;

    combat_hit_data target_data;

} combat_player;

struct bullet_data 
{
    combat_player* player;
    struct vector2 origin, dir;
};

combat_player* player_create(struct vector2 origin, float rot);
void player_update_behavior(entity id);

void bullet_create(const struct vector2 origin, struct vector2 dir, void* data);
void bullet_update_behavior(entity id);

#endif

