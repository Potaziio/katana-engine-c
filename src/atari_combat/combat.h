#ifndef COMBAT_H
#define COMBAT_H

#include "../engine/core/engine.h"
#include "player/player.h"

typedef struct 
{
    struct texture red_tank_texture;
    struct texture blue_tank_texture;
    combat_player* player_red;
    combat_player* player_blue;
} combat_game_data;

extern combat_game_data combat_data;

void combat_start(void);
void combat_update(void);
void combat_end(void);

#endif

