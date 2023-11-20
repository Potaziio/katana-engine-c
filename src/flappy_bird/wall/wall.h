#ifndef WALL_H
#define WALL_H

#include "../../engine/core/engine.h"
#include "../player/player.h"

// Goes for bottom and top walls
#define WALL_NUMBER 7

// Minimum wall height
#define WALL_MIN_Y_SCALE 150
#define WALL_MAX_Y_SCALE 330

// Distance from wall to wall
#define WALL_DISTANCE 200

#define WALL_WIDTH 40

// How far to the right from the center of the screen the walls spawn
#define WALL_CENTER_PADDING 300

// Horizontal speed of the wall 
#define WALL_SPEED 400

// Wall padding from left of screen when it wraps around
#define WALL_WRAP_LEFT_PADDING 50

enum GAME_STATE
{   
    GAME_STATE_MAIN_MENU,
    GAME_STATE_START,
};

typedef struct 
{
    enum GAME_STATE game_state;
    player* player;
} game_state;

extern struct texture wall_texture;
extern game_state state; 

void walls_create(game_state* game_state);
void wall_update_script(entity e);

#endif

