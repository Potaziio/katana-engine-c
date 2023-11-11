#include "map_editor.h"

entity horizontal_lines[MAP_EDITOR_GRID_HORIZONTAL_LINES];
entity vertical_lines[MAP_EDITOR_GRID_VERTICAL_LINES]; 

int map[MAP_EDITOR_GRID_HORIZONTAL_LINES * MAP_EDITOR_GRID_VERTICAL_LINES];

struct texture* tile_textures[TILE_TYPE_SIZE];

struct texture grass_texture;
struct texture cobble_texture;

int point_chosen;

entity p1 = 0;
entity p2 = 0;

enum TILE_TYPES selected_tile;

void replace_tile(entity e, int x, int y, enum TILE_TYPES tile)
{
	struct textured_sprite2d* sprite = ENTITY_GET_TEXTURED_SPRITE2D(e);
	map[y * MAP_EDITOR_GRID_VERTICAL_LINES + x] = tile;
	sprite->texture = *tile_textures[tile - 1];
}

void set_tile(int x, int y, enum TILE_TYPES tile)
{
	// We check if the tile we want to replace is empty, if it is, we pop the entity on the x and y, and we replace to empty
	if (tile == TILE_TYPE_EMPTY)
	{
		ENTITY_FOR_LOOP(i)
		{
			entity e = global_engine->entities[i];
			struct transform* t = ENTITY_GET_TRANSFORM(e);
			if ((t != NULL) && t->position.x == x * MAP_EDITOR_GRID_SCALE && t->position.y == y * MAP_EDITOR_GRID_SCALE)
			{
				engine_pop_entity(global_engine, e);
				map[y * MAP_EDITOR_GRID_VERTICAL_LINES + x] = TILE_TYPE_EMPTY;
				break;
			}
		}

		return;
	} 

	// If it is not empty we just replace the tile instead of setting it  
	if (map[y * MAP_EDITOR_GRID_VERTICAL_LINES + x] != TILE_TYPE_EMPTY)
	{
		ENTITY_FOR_LOOP(i)
		{
			entity e = global_engine->entities[i];
			struct transform* t = ENTITY_GET_TRANSFORM(e);
			if ((t != NULL) && t->position.x == x * MAP_EDITOR_GRID_SCALE && t->position.y == y * MAP_EDITOR_GRID_SCALE)
			{
				replace_tile(e, x, y, tile);
				break;
			}
		}
		return;
	}

	// We finally set the tile if its empty
	entity e = engine_create_entity(global_engine, TRANSFORM | TEXTURED_SPRITE2D);
	struct transform* t = ENTITY_GET_TRANSFORM(e);
	struct textured_sprite2d* s = ENTITY_GET_TEXTURED_SPRITE2D(e);
	t->position = (struct vector3){x * MAP_EDITOR_GRID_SCALE, y * MAP_EDITOR_GRID_SCALE, 0.0f};
	t->scale = (struct vector3){MAP_EDITOR_GRID_SCALE, MAP_EDITOR_GRID_SCALE, 0.0f};
	s->texture = *tile_textures[tile - 1];
	s->config = SPRITE2D_TOP_LEFT;
	map[y * MAP_EDITOR_GRID_VERTICAL_LINES + x] = tile;
}


void map_editor_set_layout(void)
{
	point_chosen = 0;

	tile_textures[0] = &grass_texture;
	tile_textures[1] = &cobble_texture;

	// We render lines for our grid
	// Horizontal lines
	for (int i = 0; i < MAP_EDITOR_GRID_HORIZONTAL_LINES; i++)
	{
		horizontal_lines[i] = engine_create_entity(global_engine, DEBUG_LINE);
		struct debug_line* line = ENTITY_GET_DEBUG_LINE(horizontal_lines[i]);

		line->vertices[0].position = (struct vector2){0.0f, i * MAP_EDITOR_GRID_SCALE};
		line->vertices[0].color = (struct rgba_color){1, 1, 1, 1};
		line->vertices[1].position = (struct vector2){global_engine->engine_window.width, i * MAP_EDITOR_GRID_SCALE};
		line->vertices[1].color = (struct rgba_color){1, 1, 1, 1};
	}

	// Vertical lines
	for (int i = 0; i < MAP_EDITOR_GRID_VERTICAL_LINES; i++)
	{
		vertical_lines[i] = engine_create_entity(global_engine, DEBUG_LINE);
		struct debug_line* line = ENTITY_GET_DEBUG_LINE(vertical_lines[i]);

		line->vertices[0].position = (struct vector2){i * MAP_EDITOR_GRID_SCALE, 0.0f};
		line->vertices[0].color = (struct rgba_color){1, 1, 1, 1};
		line->vertices[1].position = (struct vector2){i * MAP_EDITOR_GRID_SCALE, global_engine->engine_window.height};
		line->vertices[1].color = (struct rgba_color){1, 1, 1, 1};
	}

	texture_load(&grass_texture, "../src/app/assets/textures/grass.png");
	texture_load(&cobble_texture, "../src/app/assets/textures/cobble.png");
	
	// Here we set memory for our map_string 
	char map_string[MAP_EDITOR_GRID_HORIZONTAL_LINES * MAP_EDITOR_GRID_VERTICAL_LINES + 2];

	// Read the file
	io_utils_read_file("../src/app/assets/maps/map1.mp", map_string);

	// Parse the file
	for (int y = 0; y < MAP_EDITOR_GRID_HORIZONTAL_LINES; y++)
	{
		for (int x = 0; x < MAP_EDITOR_GRID_VERTICAL_LINES; x++)
		{
			char tile = map_string[y * MAP_EDITOR_GRID_VERTICAL_LINES + x];
			// We substract '0' to convert from char to int 
			if (tile - '0' == 0) continue;
			set_tile(x, y, tile - '0');
		}
	}
	// This is just for our tile replacing feature

	selected_tile = 1;
}

void map_editor_on_update(void)
{
	if (input_get_key_down(GLFW_KEY_SPACE))
	{
		// ADD
		int grid_x = (int)input_get_mouse_pos().x / MAP_EDITOR_GRID_SCALE;
		int grid_y = (int)input_get_mouse_pos().y / MAP_EDITOR_GRID_SCALE;

		if (map[grid_y * MAP_EDITOR_GRID_VERTICAL_LINES + grid_x] == TILE_TYPE_EMPTY)
			set_tile(grid_x, grid_y, TILE_TYPE_GRASS);
		else
		{
			ENTITY_FOR_LOOP(i)
			{
				entity e = global_engine->entities[i];
				struct transform* t = ENTITY_GET_TRANSFORM(e);
				if ((t != NULL) && t->position.x == grid_x * MAP_EDITOR_GRID_SCALE && t->position.y == grid_y * MAP_EDITOR_GRID_SCALE)
				{
					switch (map[grid_y * MAP_EDITOR_GRID_VERTICAL_LINES + grid_x])
					{
						case TILE_TYPE_GRASS:
							replace_tile(e, grid_x, grid_y, TILE_TYPE_COBBLESTONE);
							break;
						case TILE_TYPE_COBBLESTONE:
							replace_tile(e, grid_x, grid_y, TILE_TYPE_GRASS);
							break;
					}

					// Exit if it finds entity 
					break;
				}
			}
		}
	} 
	else if (input_get_mouse_button_down(GLFW_MOUSE_BUTTON_RIGHT))
	{
		// Deleting tile
		int grid_x = (int)input_get_mouse_pos().x / MAP_EDITOR_GRID_SCALE;
		int grid_y = (int)input_get_mouse_pos().y / MAP_EDITOR_GRID_SCALE;

		if (map[grid_y * MAP_EDITOR_GRID_VERTICAL_LINES + grid_x] != TILE_TYPE_EMPTY)
		{
			// We have to go through every entity, this is bad, but its what it is, ill make a better function for this 
			ENTITY_FOR_LOOP(i)
			{
				// We go though all entities and find the one on the cursor, then we pop and set array index to empy
				entity e = global_engine->entities[i];
				struct transform* t = ENTITY_GET_TRANSFORM(e);
				if ((t != NULL) && t->position.x == grid_x * MAP_EDITOR_GRID_SCALE && t->position.y == grid_y * MAP_EDITOR_GRID_SCALE)
				{
					engine_pop_entity(global_engine, e);
					map[grid_y * MAP_EDITOR_GRID_VERTICAL_LINES + grid_x] = TILE_TYPE_EMPTY;
				}
			}
		}
	}

	// This is to set our points for square editing
	if (input_get_key_down(GLFW_KEY_P))
	{
		// Get point on mouse
		int grid_x = (int)input_get_mouse_pos().x / MAP_EDITOR_GRID_SCALE;
		int grid_y = (int)input_get_mouse_pos().y / MAP_EDITOR_GRID_SCALE;

		// Red is first point, green second
		// point_chosen = 0 : Red 
		// 				  1 : Green

		// P1 checks if we havent cleared our selection before being able to put a different point 
		if (point_chosen == 0 && p1 == 0)
		{
			// Just puts a red square on the center of tile we selected
			p1 = engine_create_entity(global_engine, TRANSFORM | SPRITE2D);
			struct transform* t1 = ENTITY_GET_TRANSFORM(p1);
			struct sprite2d* s1 = ENTITY_GET_SPRITE2D(p1);
			t1->position = (struct vector3){grid_x * MAP_EDITOR_GRID_SCALE + MAP_EDITOR_GRID_SCALE * 0.5f, 
				grid_y * MAP_EDITOR_GRID_SCALE + MAP_EDITOR_GRID_SCALE * 0.5f, 0.0f};
			t1->scale = (struct vector3){MAP_EDITOR_GRID_SCALE * 0.5f, MAP_EDITOR_GRID_SCALE * 0.5f, 0.0f};
			s1->config = SPRITE2D_CENTERED;

			s1->color = (struct rgba_color){RGBA_RED};
			point_chosen = 1;
		} 
		else if (point_chosen == 1 && p2 == 0)
		{
			// Puts a green square on the center of tile we selected
			p2 = engine_create_entity(global_engine, TRANSFORM | SPRITE2D);
			struct transform* t2 = ENTITY_GET_TRANSFORM(p2);
			struct sprite2d* s2 = ENTITY_GET_SPRITE2D(p2);
			t2->position = (struct vector3){grid_x * MAP_EDITOR_GRID_SCALE + MAP_EDITOR_GRID_SCALE * 0.5f, 
				grid_y * MAP_EDITOR_GRID_SCALE + MAP_EDITOR_GRID_SCALE * 0.5f, 0.0f};
			t2->scale = (struct vector3){MAP_EDITOR_GRID_SCALE * 0.5f, MAP_EDITOR_GRID_SCALE * 0.5f, 0.0f};
			s2->config = SPRITE2D_CENTERED;
			s2->color = (struct rgba_color){RGBA_GREEN};
		}
	}

	// Fill the rectangles
	if (input_get_key_down(GLFW_KEY_J))
	{
		if (point_chosen == 1)
		{
			// Reset
			struct transform* t1 = ENTITY_GET_TRANSFORM(p1);
			struct transform* t2 = ENTITY_GET_TRANSFORM(p2);

			// Rectangle filling code

			//  Gets highest between to points
			int highest = t1->position.y < t2->position.y ? 0 : 1;

			// Get each index position of each point
			int p1_x = (int)t1->position.x / MAP_EDITOR_GRID_SCALE;
			int p1_y = (int)t1->position.y / MAP_EDITOR_GRID_SCALE;

			int p2_x = (int)t2->position.x / MAP_EDITOR_GRID_SCALE;
			int p2_y = (int)t2->position.y / MAP_EDITOR_GRID_SCALE;

			// gets distance, is not absolute, can be negative
			int cell_x_dist = p2_x - p1_x;
			int cell_y_dist = p2_y - p1_y;

			// For y we set y to the highest points y position, then we add 1 to y until its equal to the distance of the points,
			// we add one or substract one from the distance depending on which one is highest, this is to make sure we fill the row where the mouse is too
			for (int y = highest == 0 ? p1_y : p2_y; y < (highest == 0 ? p1_y : p2_y) + abs(cell_y_dist + (highest == 0 ? 1 : -1)); y++)
			{
				if (cell_x_dist >= 0)
				{		
					for (int x = p1_x; x < p1_x + cell_x_dist + 1; x++)
					{
						// Sets all horizontal 
						set_tile(x, y, selected_tile);
					}
				}
				else if (cell_x_dist < 0)
				{
					// Sets all horizontal tiles
					for (int x = p1_x; x > p1_x - abs(cell_x_dist - 1); x--)
						set_tile(x, y, selected_tile);
				}
			}

			// Removes the points after were done 
			engine_pop_entity(global_engine, p1);
			engine_pop_entity(global_engine, p2);
			p1 = 0;
			p2 = 0;
			point_chosen = 0;
		}
	}

	// Cycles through our selected tile 
	if (input_get_key_down(GLFW_KEY_RIGHT))
	{
		if (selected_tile <	TILE_TYPE_SIZE - 1)
			selected_tile++;
		else 
			selected_tile = 0;
		
		logger_log_string(LOG, "Selected tile: ");
		printf("%d\n", selected_tile);
	}
	else if (input_get_key_down(GLFW_KEY_LEFT))
	{
		if (selected_tile > 0)
			selected_tile--;
		else
			selected_tile = TILE_TYPE_SIZE - 1;

		logger_log_string(LOG, "Selected tile: ");
		printf("%d\n", selected_tile);
	}
}

void map_editor_serialize(void)
{
	logger_log_string(LOG, "Serializing map\n");

	// We just serialize our map when were done, just writes a string to our file
		
	char map_string[MAP_EDITOR_GRID_VERTICAL_LINES * MAP_EDITOR_GRID_HORIZONTAL_LINES + 1];

	for (int i = 0; i < MAP_EDITOR_GRID_VERTICAL_LINES * MAP_EDITOR_GRID_HORIZONTAL_LINES; i++)
		map_string[i] =  map[i] + '0';

	map_string[strlen(map_string)] = '\0';

	io_utils_write_to_file("../src/app/assets/maps/map1.mp", map_string);

	logger_log_string(SUCCESS, "Map serialized successfully\n");

	texture_free(&grass_texture);
	texture_free(&cobble_texture);
}

