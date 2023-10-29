#ifndef MAP_H
#define MAP_H

#define GRAVITY 1.5f
#define MAP_EDITOR_GRID_VERTICAL_LINES 45
#define MAP_EDITOR_GRID_HORIZONTAL_LINES 24
#define MAP_EDITOR_GRID_SCALE 32

enum TILE_TYPES
{
	TILE_TYPE_EMPTY = 0,
	TILE_TYPE_GRASS,
	TILE_TYPE_COBBLESTONE,
	TILE_TYPE_SIZE,
};

extern struct texture grass_texture;
extern struct texture cobble_texture;

extern struct texture* tile_textures[TILE_TYPE_SIZE];

extern int map[MAP_EDITOR_GRID_HORIZONTAL_LINES * MAP_EDITOR_GRID_VERTICAL_LINES];

#endif

