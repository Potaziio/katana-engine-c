#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "../../../engine/core/engine.h"
#include "../../../engine/entity/entity.h"
#include "../map.h"
#include <stdio.h>
#include <stdlib.h>

void replace_tile(entity e, int x, int y, enum TILE_TYPES tile);
void set_tile(int x, int y, enum TILE_TYPES tile);
void map_editor_set_layout(void);
void map_editor_on_update(void);
void map_editor_serialize(void);

#endif

