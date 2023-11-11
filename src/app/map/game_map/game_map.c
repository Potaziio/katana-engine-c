#include "game_map.h"

void game_map_create(struct game_map* game_map)
{
	game_map->map_path = "../src/app/assets/maps/map1.mp";
	game_map->background_path = "../src/app/assets/textures/background2.png";

	tile_textures[0] = &grass_texture;
	tile_textures[1] = &cobble_texture;

	game_map->colliders.size = 1;

	game_map->colliders.colliders = (struct aabb*)malloc(sizeof(struct aabb) * game_map->colliders.size);

	texture_load(&grass_texture, "../src/app/assets/textures/grass.png");
	texture_load(&cobble_texture, "../src/app/assets/textures/cobble.png");

	// Here we set memory for our map_string 
	char map_string[MAP_EDITOR_GRID_HORIZONTAL_LINES * MAP_EDITOR_GRID_VERTICAL_LINES + 2];

	// Read the file
	io_utils_read_file(game_map->map_path, map_string);

	/* map_string[MAP_EDITOR_GRID_HORIZONTAL_LINES * MAP_EDITOR_GRID_VERTICAL_LINES + 1] = '\0'; */

	entity background = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D);
	struct texture bg;
	texture_load(&bg, game_map->background_path);
	(ENTITY_GET_TEXTURED_SPRITE2D(background))->texture = bg;
	(ENTITY_GET_TEXTURED_SPRITE2D(background))->config = SPRITE2D_TOP_LEFT;
	(ENTITY_GET_TRANSFORM(background))->scale = (struct vector3){engine_camera.bounds.x, engine_camera.bounds.y, 0.0f};

	// Parse the file
	for (int y = 0; y < MAP_EDITOR_GRID_HORIZONTAL_LINES; y++)
	{
		for (int x = 0; x < MAP_EDITOR_GRID_VERTICAL_LINES; x++)
		{
			char tile = map_string[y * MAP_EDITOR_GRID_VERTICAL_LINES + x];

			// We substract '0' to convert from char to int 
			if (tile - '0' == TILE_TYPE_EMPTY) continue;

			struct aabb* collider = &game_map->colliders.colliders[game_map->colliders.size - 1];
			collider->position = (struct vector2){x * MAP_EDITOR_GRID_SCALE, y * MAP_EDITOR_GRID_SCALE};
			collider->scale = (struct vector2){MAP_EDITOR_GRID_SCALE, MAP_EDITOR_GRID_SCALE};
			collider->tile = tile - '0';

			entity e = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D);
			struct transform* t = ENTITY_GET_TRANSFORM(e);
			struct textured_sprite2d* s = ENTITY_GET_TEXTURED_SPRITE2D(e);
			t->position = (struct vector3){x * MAP_EDITOR_GRID_SCALE, y * MAP_EDITOR_GRID_SCALE, 0.0f};
			t->scale = (struct vector3){MAP_EDITOR_GRID_SCALE, MAP_EDITOR_GRID_SCALE, 0.0f};
			s->texture = *tile_textures[(tile - '0') - 1];
			s->config = SPRITE2D_TOP_LEFT;
			map[y * MAP_EDITOR_GRID_VERTICAL_LINES + x] = tile;

			game_map->colliders.size++;
			game_map->colliders.colliders = (struct aabb*)realloc(game_map->colliders.colliders, sizeof(struct aabb) * game_map->colliders.size);
		}
	}
}
