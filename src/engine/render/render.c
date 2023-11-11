#include "render.h"

// This is just some opengl boiler plate code but we follow the same pattern when rendering something.
// First we generate data, vbos, vaos, ebos (if necessary)
// Then we set the data, add vertices to vbos, indices to ebos
// Set vertex attributes 
// bind data
// render
// unbind data

/*
	SHADER HAS TO BE IN USE FIRST BEFORE RENDERING ANYTHING, RENDERING FUNCTIONS DO NOT KNOW ABOUT SHADERS THEREFORE YOU HAVE TO MAKE SURE THAT 
	THE SHADER IS BEING USED.
*/

struct shader* _render_default_shader;
struct shader* _render_default_tex_shader;
struct shader* _render_line_shader;
struct shader* _render_batch_simple_shader;
struct shader* _render_font_shader;
struct texture* _render_font_default_texture;

void render_system_init_sprite2d(struct transform_hashmap* transform_map, struct sprite2d_hashmap* sprite2d_map,  entity entity)
{
	struct sprite2d* sprite = sprite2d_hashmap_get(sprite2d_map, entity);

	if (sprite == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	if (sprite->was_initialized) return;

	if (sprite->config & SPRITE2D_CENTERED)
	{
		sprite->vertices[0].position = (struct vector3){-0.5f, -0.5f, 0.0f};
		sprite->vertices[1].position = (struct vector3){0.5f, -0.5f, 0.0f};
		sprite->vertices[2].position = (struct vector3){-0.5f, 0.5f, 0.0f};
		sprite->vertices[3].position = (struct vector3){0.5f, 0.5f, 0.0f};
	} 
	else if (sprite->config & SPRITE2D_TOP_LEFT)
	{
		sprite->vertices[0].position = (struct vector3){0.0f, 0.0f, 0.0f};
		sprite->vertices[1].position = (struct vector3){1.0f, 0.0f, 0.0f};
		sprite->vertices[2].position = (struct vector3){0.0f, 1.0f, 0.0f};
		sprite->vertices[3].position = (struct vector3){1.0f, 1.0f, 0.0f};
	} 
	else
	{
		sprite->vertices[0].position = (struct vector3){-0.5f, -0.5f, 0.0f};
		sprite->vertices[1].position = (struct vector3){0.5f, -0.5f, 0.0f};
		sprite->vertices[2].position = (struct vector3){-0.5f, 0.5f, 0.0f};
		sprite->vertices[3].position = (struct vector3){0.5f, 0.5f, 0.0f};

		logger_log_string(WARNING, "Origin configuration for sprite is bad, generating sprite at center (default)\n");
		sprite->config |= SPRITE2D_CENTERED;
	}

	
	for (int i = 0; i < 4; i++)
		sprite->vertices[i].color = (struct rgba_color){RGBA_NORMALIZED_INT(sprite->color)};

	sprite->indices[0] = 0;
	sprite->indices[1] = 1;
	sprite->indices[2] = 2;
	sprite->indices[3] = 2;
	sprite->indices[4] = 3;
	sprite->indices[5] = 1;

	glGenBuffers(1, &sprite->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
	glGenVertexArrays(1, &sprite->vao);
	glBindVertexArray(sprite->vao);
	glGenBuffers(1, &sprite->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(struct rectangle_sprite_vertex) * 4, sprite->vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, sprite->indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct rectangle_sprite_vertex), (void*)0);
	glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, sizeof(struct rectangle_sprite_vertex), (void*)(3 * sizeof(float)));

	sprite->was_initialized = 1;
}

void render_system_render_sprite2d(struct transform_hashmap* transform_map, struct sprite2d_hashmap* sprite2d_map, entity entity)
{
	struct sprite2d* sprite = sprite2d_hashmap_get(sprite2d_map, entity);
	struct transform* transform = transform_hashmap_get(transform_map, entity);

	if (sprite == NULL || transform == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	vec3 glm_pos_vec = {transform->position.x, transform->position.y, 0.0f};
	vec3 glm_scale_vec = {transform->scale.x, transform->scale.y, 0.0f};
	vec3 glm_rotation_vec = {0.0f, 0.0f, transform->rotation_z};

	glm_mat4_identity(transform->matrix);
	glm_translate(transform->matrix, glm_pos_vec);
	glm_rotate(transform->matrix, glm_rad(transform->rotation_angle), glm_rotation_vec);
	glm_scale(transform->matrix, glm_scale_vec);

	if (sprite->config & SPRITE2D_WIREFRAME) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shader_send_mat4(_render_default_shader, transform->matrix, "transform");

	glBindVertexArray(sprite->vao);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);;
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (sprite->config & SPRITE2D_WIREFRAME) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void render_system_update_sprite2d_verts(struct sprite2d_hashmap* sprite2d_map, entity entity)
{
	// Updates color automatically

	struct sprite2d* sprite = sprite2d_hashmap_get(sprite2d_map, entity);

	if (sprite == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	for (int i = 0; i < SPRITE2D_VERTEX_NUM; i++)
		sprite->vertices[i].color = (struct rgba_color){RGBA_NORMALIZED(sprite->color)};

	glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct rectangle_sprite_vertex) * 4, sprite->vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render_system_init_textured_sprite2d(struct transform_hashmap* transform_map, struct textured_sprite2d_hashmap* sprite2d_map, entity entity)
{
	struct textured_sprite2d* sprite = textured_sprite2d_hashmap_get(sprite2d_map, entity);

	if (sprite == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	if (sprite->was_initialized) return;

	if (sprite->config & SPRITE2D_CENTERED)
	{
		sprite->vertices[0].position = (struct vector3){-0.5f, -0.5f, 0.0f};
		sprite->vertices[1].position = (struct vector3){0.5f, -0.5f, 0.0f};
		sprite->vertices[2].position = (struct vector3){-0.5f, 0.5f, 0.0f};
		sprite->vertices[3].position = (struct vector3){0.5f, 0.5f, 0.0f};
	} else if (sprite->config & SPRITE2D_TOP_LEFT)
	{
		sprite->vertices[0].position = (struct vector3){0.0f, 0.0f, 0.0f};
		sprite->vertices[1].position = (struct vector3){1.0f, 0.0f, 0.0f};
		sprite->vertices[2].position = (struct vector3){0.0f, 1.0f, 0.0f};
		sprite->vertices[3].position = (struct vector3){1.0f, 1.0f, 0.0f};
	}
	else
	{
		sprite->vertices[0].position = (struct vector3){-0.5f, -0.5f, 0.0f};
		sprite->vertices[1].position = (struct vector3){0.5f, -0.5f, 0.0f};
		sprite->vertices[2].position = (struct vector3){-0.5f, 0.5f, 0.0f};
		sprite->vertices[3].position = (struct vector3){0.5f, 0.5f, 0.0f};

		logger_log_string(WARNING, "Origin configuration for sprite is bad, generating sprite at center (default)\n");
		sprite->config |= SPRITE2D_CENTERED;
	}


	sprite->vertices[0].tex_coords = (struct vector2){0.0f, 1.0f};
	sprite->vertices[1].tex_coords = (struct vector2){1.0f, 1.0f};
	sprite->vertices[2].tex_coords = (struct vector2){0.0f, 0.0f};
	sprite->vertices[3].tex_coords = (struct vector2){1.0f, 0.0f};

	/* texture_load(&sprite->texture, sprite->texture.file_path); */

	sprite->indices[0] = 0;
	sprite->indices[1] = 1;
	sprite->indices[2] = 2;
	sprite->indices[3] = 2;
	sprite->indices[4] = 3;
	sprite->indices[5] = 1;

	glGenBuffers(1, &sprite->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
	glGenVertexArrays(1, &sprite->vao);
	glBindVertexArray(sprite->vao);
	glGenBuffers(1, &sprite->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(struct textured_rectangle_sprite_vertex) * 4, sprite->vertices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, sprite->indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct textured_rectangle_sprite_vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct textured_rectangle_sprite_vertex), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	sprite->was_initialized = 1;
}

void render_system_render_textured_sprite2d(struct transform_hashmap* transform_map, struct textured_sprite2d_hashmap* sprite2d_map, entity entity)
{
	struct textured_sprite2d* sprite = textured_sprite2d_hashmap_get(sprite2d_map, entity);
	struct transform* transform = transform_hashmap_get(transform_map, entity);

	if (sprite == NULL || transform == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	vec3 glm_pos_vec = {transform->position.x, transform->position.y, 0.0f};
	vec3 glm_scale_vec = {transform->scale.x, transform->scale.y, 0.0f};
	vec3 glm_rotation_vec = {0.0f, 0.0f, transform->rotation_z};

	glm_mat4_identity(transform->matrix);
	glm_translate(transform->matrix, glm_pos_vec);
	glm_rotate(transform->matrix, glm_rad(transform->rotation_angle), glm_rotation_vec);
	glm_scale(transform->matrix, glm_scale_vec);

	shader_send_mat4(_render_default_tex_shader, transform->matrix, "transform");

	texture_bind(sprite->texture);
	glBindVertexArray(sprite->vao);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	texture_unbind();
}

void render_textured_sprite_set_sprite(struct animation animation, int index, struct textured_sprite2d* sprite)
{
	glBindVertexArray(sprite->vao);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->ebo);

	sprite->vertices[0].tex_coords = (struct vector2){0.0f + (index * ((animation.sprite_width + animation.sprite_padding) / animation.spritesheet_width)), 1.0f};
	sprite->vertices[1].tex_coords = (struct vector2){sprite->vertices[0].tex_coords.x + ((animation.sprite_width - animation.sprite_padding) / animation.spritesheet_width), 1.0f};
	sprite->vertices[2].tex_coords = (struct vector2){sprite->vertices[0].tex_coords.x, 0.0f};
	sprite->vertices[3].tex_coords = (struct vector2){sprite->vertices[1].tex_coords.x, 0.0f};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct textured_rectangle_sprite_vertex) * 4, sprite->vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void animation_cycle_through(struct animation* animation, struct textured_sprite2d* sprite, struct frame_timer* timer)
{
	if (animation->current_index < animation->start)
		animation->current_index = animation->start;
			
	int x = animation->current_index;

	if (frame_timer_add_and_respond(timer))
	{
		render_textured_sprite_set_sprite(*animation, x, sprite);

		if (animation->current_index < animation->end)
			animation->current_index++;
		else 
			animation->current_index = animation->start;
	}
}


// Vertices have to be allocated already
void render_system_init_sprite2d_batch_simple(struct sprite2d_batch_simple_hashmap* batch_map, entity entity)
{
	struct sprite2d_batch_simple* batch = sprite2d_batch_simple_hashmap_get(batch_map, entity);

	if (batch == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	if (batch->was_initialized) return;

	glGenBuffers(1, &batch->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glGenVertexArrays(1, &batch->vao);
	glBindVertexArray(batch->vao);
	glGenBuffers(1, &batch->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(struct rectangle_sprite_vertex) * 4 * batch->batch_size, batch->vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6 * batch->batch_size, batch->indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct rectangle_sprite_vertex), (void*)0);
	glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, sizeof(struct rectangle_sprite_vertex), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	batch->was_initialized = 1;
}

void render_system_render_sprite2d_batch_simple(struct sprite2d_batch_simple_hashmap* batch_map, entity entity)
{
	struct sprite2d_batch_simple* batch = sprite2d_batch_simple_hashmap_get(batch_map, entity);

	if (batch == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	glBindVertexArray(batch->vao);
	glBindBuffer(GL_ARRAY_BUFFER, batch->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch->ebo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 6 * batch->batch_size, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void render_system_init_debug_line(struct debug_line_hashmap* line_map, entity entity)
{
	struct debug_line* line = debug_line_hashmap_get(line_map, entity);

	if (line == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	if (line->was_initialized) return;

	glGenBuffers(1, &line->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, line->vbo);

	glGenVertexArrays(1, &line->vao);
	glBindVertexArray(line->vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(struct line_vertex) * 2, line->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct line_vertex), (void*)0);
	glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, sizeof(struct line_vertex), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	line->was_initialized = 1;
}

void render_system_update_debug_line(struct debug_line_hashmap* line_map, entity entity)
{
	struct debug_line* line = debug_line_hashmap_get(line_map, entity);
	glBindBuffer(GL_ARRAY_BUFFER, line->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct line_vertex) * 2, line->vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render_system_render_debug_line(struct debug_line_hashmap* line_map, entity entity)
{
	struct debug_line* line = debug_line_hashmap_get(line_map, entity);

	if (line == NULL)
	{
		logger_log_string(ERROR, "Render: Bad access\n");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, line->vbo);
	glBindVertexArray(line->vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// These two functions dont require the creation of a debug line entity, just a debug line struct 

void render_draw_debug_line(struct debug_line* line)
{
	glBindBuffer(GL_ARRAY_BUFFER, line->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(struct line_vertex) * 2, line->vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, line->vbo);
	glBindVertexArray(line->vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render_init_debug_line(struct debug_line* line)
{
	glGenBuffers(1, &line->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, line->vbo);

	glGenVertexArrays(1, &line->vao);
	glBindVertexArray(line->vao);

	glBufferData(GL_ARRAY_BUFFER, sizeof(struct line_vertex) * 2, line->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct line_vertex), (void*)0);
	glVertexAttribPointer(1, 4, GL_BYTE, GL_FALSE, sizeof(struct line_vertex), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render_free_debug_line(struct debug_line *line)
{
	glDeleteBuffers(1, &line->vbo);
	glDeleteVertexArrays(1, &line->vao);
}

void render_init_debug_aabb_collider(struct aabb aabb, struct debug_line* lines)
{
	lines[0].vertices[0].position = (struct vector2){aabb.position.x, aabb.position.y};
	lines[0].vertices[1].position = (struct vector2){aabb.position.x + aabb.scale.x, aabb.position.y};

	lines[1].vertices[0].position = lines[0].vertices[0].position;
	lines[1].vertices[1].position = (struct vector2){aabb.position.x, aabb.position.y + aabb.scale.y};

	lines[2].vertices[0].position = lines[1].vertices[1].position;
	lines[2].vertices[1].position = (struct vector2){aabb.position.x + aabb.scale.x, aabb.position.y + aabb.scale.y};

	lines[3].vertices[0].position = lines[2].vertices[1].position;
	lines[3].vertices[1].position = lines[0].vertices[1].position;


	for (int i = 0; i < 4; i++)
		render_init_debug_line(&lines[i]);
}

void render_draw_debug_aabb_collider(struct aabb aabb, struct debug_line* lines)
{
	lines[0].vertices[0].position = (struct vector2){aabb.position.x, aabb.position.y};
	lines[0].vertices[1].position = (struct vector2){aabb.position.x + aabb.scale.x, aabb.position.y};

	lines[1].vertices[0].position = lines[0].vertices[0].position;
	lines[1].vertices[1].position = (struct vector2){aabb.position.x, aabb.position.y + aabb.scale.y};

	lines[2].vertices[0].position = lines[1].vertices[1].position;
	lines[2].vertices[1].position = (struct vector2){aabb.position.x + aabb.scale.x, aabb.position.y + aabb.scale.y};

	lines[3].vertices[0].position = lines[2].vertices[1].position;
	lines[3].vertices[1].position = lines[0].vertices[1].position;

	shader_use(_render_line_shader);
	for (int i = 0; i < 4; i++)
		render_draw_debug_line(&lines[i]);
	shader_detach(_render_line_shader);
}

void render_free_debug_aabb_collider(struct aabb aabb, struct debug_line* lines)
{
	for (int i = 0; i < 4; i++)
		render_free_debug_line(&lines[i]);
}


